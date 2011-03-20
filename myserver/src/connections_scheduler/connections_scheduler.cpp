/*
  MyServer
  Copyright (C) 2007, 2008, 2009, 2010, 2011 Free Software Foundation,
  Inc.
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "myserver.h"

#ifdef WIN32
# include <w32sock.h>

#endif

#include <include/connections_scheduler/connections_scheduler.h>
#include <include/base/exceptions/exceptions.h>
#include <include/server/server.h>

static DEFINE_THREAD (dispatcher, p)
{
  ConnectionsScheduler::DispatcherArg *da
    = (ConnectionsScheduler::DispatcherArg*) p;

  if (da == NULL)
    return NULL;

  if (! da->terminated)
    return 0;

  da->terminated = false;

  while (!da->terminate)
    {
      int res;

      da->mutex->lock ();
      try
        {
          res = event_loop (EVLOOP_ONCE);
          da->mutex->unlock ();
        }
      catch (...)
        {
          da->mutex->unlock ();
        }

      if (res == 1)
        Thread::wait (10);
    }

  da->terminated = true;
  return NULL;
}

static void newDataHandler (int fd, short event, void *param)
{
  ConnectionsScheduler::DispatcherArg* arg
    = (ConnectionsScheduler::DispatcherArg*) param;

  if (!arg->terminate && arg->scheduler)
    arg->scheduler->newData (event, fd);
}


void ConnectionsScheduler::newData (short event, SocketHandle handle)
{
  ConnectionPtr connection = NULL;

  connectionsMutex.lock ();
  try
    {
      connection = connections.get ((int) handle);
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    }

  if (connection == NULL || server == NULL)
    return;

  if (connection->notifySchedulerHandler (event))
    return;

  if (event == EV_READ)
    addReadyConnection (connection);
  else if (event == EV_TIMEOUT)
    {
      if (!connection->allowDelete ())
        return;

      server->notifyDeleteConnection (connection);
      server->deleteConnection (connection);
    }
}

static void eventLoopHandler (int fd, short event, void *arg)
{
  ConnectionsScheduler::DispatcherArg *da
    = (ConnectionsScheduler::DispatcherArg*) arg;
  size_t nbr;
  timeval tv = {10, 0};

  if (event == EV_READ || event == EV_TIMEOUT)
    {
      while (da->socketPair.bytesToRead ())
        {
          char cmd;
          da->socketPair.read (&cmd, 1, &nbr);
          switch (cmd)
            {
            case 'c':
              /*
                Schedule a new connection.
                The 'c' command is followed by:
                SocketHandle  -> Socket to monitor for new data.
                ConnectionPtr -> Related Connection.
                timeval       -> Timeout.
               */
              SocketHandle handle;
              ConnectionPtr c;

              da->socketPair.read ((char*) &handle, sizeof (SocketHandle), &nbr);
              da->socketPair.read ((char*) &c, sizeof (ConnectionPtr), &nbr);
              da->socketPair.read ((char*) &tv, sizeof (timeval), &nbr);

              event_once (
#ifdef WIN32
              FD_TO_SOCKET (handle),
#else
              handle,
#endif
              EV_READ | EV_TIMEOUT, newDataHandler, da, &tv);
              break;

            case 'l':
              ConnectionsScheduler::ListenerArg *arg;
              da->socketPair.read ((char*) &arg, sizeof (arg), &nbr);
              event_add (&(arg->ev), &tv);
              break;

            case 'r':
              return;

            default:
              /* Handle other cmd with a nop.  */
              continue;
            }
        }
    }
  event_add (&(da->loopEvent), NULL);
}

static void listenerHandler (int fd, short event, void *arg)
{
  static timeval tv = {5, 0};
  ConnectionsScheduler::ListenerArg* s
    = (ConnectionsScheduler::ListenerArg*) arg;

  if (event == EV_TIMEOUT)
    s->scheduler->getReadySemaphore ()->unlock ();

  if (event == EV_READ)
    {
      MYSERVER_SOCKADDRIN asockIn;
      socklen_t asockInLen = sizeof (asockIn);
      Socket *clientSock = NULL;
      try
        {
          do
            {
              try
                {
                  clientSock = s->serverSocket->accept (&asockIn, &asockInLen);
                }
              catch (InvalidResourceException & e)
                {
                  clientSock = NULL;
                }

              if (s->server && clientSock
                  && s->server->addConnection (clientSock, &asockIn))
                {
                  clientSock->shutdown (2);
                  clientSock->close ();
                  delete clientSock;
                }
            }
          while (clientSock);
        }
      catch (...)
        {
        }
    }

  event_add (&(s->ev), &tv);
}

/*!
  Add a listener socket to the event queue.
  This is used to renew the event after the listener thread is notified.

  \param la Structure containing an Event to be notified on new data.
 */
void ConnectionsScheduler::listener (ConnectionsScheduler::ListenerArg *la)
{
  ConnectionsScheduler::ListenerArg *arg = new ConnectionsScheduler::ListenerArg (la);

    event_set (&(arg->ev),
#ifdef WIN32
              FD_TO_SOCKET (la->serverSocket->getHandle ()),
#else
              la->serverSocket->getHandle (),
#endif
              EV_READ | EV_TIMEOUT, listenerHandler, arg);

  arg->terminate = &dispatcherArg.terminate;
  arg->scheduler = this;
  arg->server = server;
  arg->eventsMutex = &eventsMutex;

  la->serverSocket->setNonBlocking (1);
  listeners.push_front (arg);

  eventsSocketMutex.lock ();
  try
    {
      size_t nbw;
      dispatcherArg.socketPairWrite.write ("l", 1, &nbw);
      dispatcherArg.socketPairWrite.write ((const char*) &arg, sizeof (arg), &nbw);
      eventsSocketMutex.unlock ();
    }
  catch (...)
    {
      eventsSocketMutex.unlock ();
      throw;
    }
}

/*!
  Remove a listener thread from the list.
 */
void ConnectionsScheduler::removeListener (ConnectionsScheduler::ListenerArg* la)
{
  eventsMutex.lock ();
  try
    {
      event_del (&(la->ev));
      listeners.remove (la);
      eventsMutex.unlock ();
    }
  catch (...)
    {
      eventsMutex.unlock ();
    }
}

/*!
  C'tor.
 */
ConnectionsScheduler::ConnectionsScheduler (Server* server)
{
  readyMutex.init ();
  eventsMutex.init ();
  connectionsMutex.init ();
  eventsSocketMutex.init ();
  readySemaphore = new Semaphore (0);
  currentPriority = 0;
  currentPriorityDone = 0;
  nTotalConnections = 0;
  ready = new queue<ConnectionPtr>[PRIORITY_CLASSES];
  this->server = server;
}

/*!
  Get the number of all connections made to the server.
 */
u_long ConnectionsScheduler::getNumTotalConnections ()
{
  return nTotalConnections;
}

/*!
  Register the connection with a new ID.
  \param connection The connection to register.
 */
void ConnectionsScheduler::registerConnectionID (ConnectionPtr connection)
{
  connectionsMutex.lock ();
  try
    {
      connection->setID (nTotalConnections++);
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    }
}

/*!
  Restart the scheduler.
 */
void ConnectionsScheduler::restart ()
{
  readyMutex.init ();
  connectionsMutex.init ();
  eventsMutex.init ();
  listeners.clear ();

  if (readySemaphore)
    delete readySemaphore;

  readySemaphore = new Semaphore (0);

  initialize ();
}

/*!
  Static initialization.
 */
void ConnectionsScheduler::initialize ()
{
  event_init ();

  dispatcherArg.terminated = true;
  dispatcherArg.terminate = false;
  dispatcherArg.mutex = &eventsMutex;
  dispatcherArg.server = server;
  dispatcherArg.scheduler = this;

  dispatchedThreadId = 0;

  int err = dispatcherArg.socketPair.create ();

  if (err == -1)
    {
      if (server)
        server->log (MYSERVER_LOG_MSG_ERROR,
                     _("Error initializing socket pair"));
      return;
    }

  dispatcherArg.socketPairWrite.setHandle (dispatcherArg.socketPair.getSecondHandle ());

  event_set (&(dispatcherArg.loopEvent),
#ifdef WIN32
             FD_TO_SOCKET (dispatcherArg.socketPair.getFirstHandle ()),
#else
             dispatcherArg.socketPair.getFirstHandle (),
#endif
             EV_READ | EV_TIMEOUT, eventLoopHandler, &dispatcherArg);

  event_add (&(dispatcherArg.loopEvent), NULL);

  if (Thread::create (&dispatchedThreadId, dispatcher, &dispatcherArg))
    {
      if (server)
        server->log (MYSERVER_LOG_MSG_ERROR,
                     _("Error while initializing the dispatcher thread"));

      dispatchedThreadId = 0;
    }

  releasing = false;
}

/*!
  D'tor.
 */
ConnectionsScheduler::~ConnectionsScheduler ()
{
  readyMutex.destroy ();
  eventsMutex.destroy ();
  eventsSocketMutex.destroy ();
  connectionsMutex.destroy ();
  delete readySemaphore;
  delete [] ready;
}


/*!
  Add an existent connection to ready connections queue.
 */
void ConnectionsScheduler::addReadyConnection (ConnectionPtr c)
{
  addReadyConnectionImpl (c);
}

/*!
  Add a new connection to ready connections queue.
 */
void ConnectionsScheduler::addNewReadyConnection (ConnectionPtr c)
{
  addReadyConnectionImpl (c);
}

/*!
  Add a connection to ready connections queue.
 */
void ConnectionsScheduler::addReadyConnectionImpl (ConnectionPtr c)
{
  int priority = c->getPriority ();

  if (priority == -1 && c->host)
    priority = c->host->getDefaultPriority ();

  priority = std::max (0, priority);
  priority = std::min (PRIORITY_CLASSES - 1, priority);

  c->setScheduled (1);

  readyMutex.lock ();
  try
    {
      ready[priority].push (c);
      readyMutex.unlock ();
    }
  catch (...)
    {
      readyMutex.unlock ();
      throw;
    }

  if (server)
    server->checkThreadsNumber ();

  readySemaphore->unlock ();
}

/*!
  Add a new connection to the scheduler.
 */
void ConnectionsScheduler::addNewWaitingConnection (ConnectionPtr c)
{
  addWaitingConnectionImpl (c, 0);
}

/*!
  Reschedule a connection in the scheduler.
 */
void ConnectionsScheduler::addWaitingConnection (ConnectionPtr c)
{
  addWaitingConnectionImpl (c, 1);
}

/*!
  Implementation to add a connection to waiting connections queue.
 */
void ConnectionsScheduler::addWaitingConnectionImpl (ConnectionPtr c, int lock)
{
  static timeval tv = {10, 0};
  int handle = c->socket->getHandle ();
  SocketHandle socketHandle;
#ifdef WIN32
  socketHandle = FD_TO_SOCKET (handle);
#else
  socketHandle = handle;
#endif

  if (server)
    tv.tv_sec = server->getTimeout () / 1000000;
  else
    tv.tv_sec = 30;

  c->setScheduled (0);

  connectionsMutex.lock ();
  try
    {
      connections.put (socketHandle, c);
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    }

  /*
    If there is need to obtain the events lock don't block the current
    thread but send the 'c' message to the eventLoopHandler function,
    it will reschedule the connection from its thread context while it
    owns the lock.
   */
  if (lock)
    {
      size_t nbw;
      eventsSocketMutex.lock ();
      try
        {
          dispatcherArg.socketPairWrite.write ("c", 1, &nbw);
          dispatcherArg.socketPairWrite.write ((const char*) &handle,
                                               sizeof (SocketHandle), &nbw);
          dispatcherArg.socketPairWrite.write ((const char*) &c,
                                               sizeof (ConnectionPtr), &nbw);
          dispatcherArg.socketPairWrite.write ((const char*) &tv,
                                               sizeof (timeval), &nbw);
          eventsSocketMutex.unlock ();
        }
      catch (...)
        {
          eventsSocketMutex.unlock ();
          throw;
        }
    }
  else
    event_once (
#ifdef WIN32
                FD_TO_SOCKET (handle),
#else
                handle,
#endif
                EV_READ | EV_TIMEOUT, newDataHandler,
                &dispatcherArg, &tv);
}

/*!
  Get a connection from the active connections queue.
 */
ConnectionPtr ConnectionsScheduler::getConnection ()
{
  ConnectionPtr ret = 0;

  if (releasing)
    return NULL;

  readySemaphore->lock ();

  if (releasing)
    return NULL;

  readyMutex.lock ();
  try
    {
      for (int i = 0; i < PRIORITY_CLASSES; i++)
        {
          if (currentPriorityDone > currentPriority ||
              !ready[currentPriority].size ())
            {
              currentPriority = (currentPriority + 1) % PRIORITY_CLASSES;
              currentPriorityDone = 0;
            }

          if (ready[currentPriority].size ())
            {
              ret = ready[currentPriority].front ();
              ret->setScheduled (0);
              ready[currentPriority].pop ();
              currentPriorityDone++;
              break;
            }
        }
      readyMutex.unlock ();
    }
  catch (...)
    {
      readyMutex.unlock ();
      throw;
    }
  return ret;
}

/*!
  Release all the blocking calls.
 */
void ConnectionsScheduler::release ()
{
  size_t nbw;
  u_long threads = 0;
  releasing = true;
  dispatcherArg.terminate = true;

  if (server)
    server->getThreadsNumberInformation (&threads);
  threads *= 2;

  for (u_long i = 0; i < threads; i++)
    readySemaphore->unlock ();

  eventsSocketMutex.lock ();
  try
    {
      dispatcherArg.socketPairWrite.write ("r", 1, &nbw);
      eventsSocketMutex.unlock ();
    }
  catch (...)
    {
      eventsSocketMutex.unlock ();
      throw;
    }

  if (dispatchedThreadId)
    Thread::join (dispatchedThreadId);

  terminateConnections ();

  eventsMutex.lock ();
  try
    {
      event_del (&(dispatcherArg.loopEvent));
      list<ListenerArg*>::iterator it = listeners.begin ();
      while (it != listeners.end ())
        {
          event_del (&((*it)->ev));
          delete *it;
          it++;
        }
      listeners.clear ();
      eventsMutex.unlock ();
    }
  catch (...)
    {
      eventsMutex.unlock ();
      throw;
    }
}

/*!
  Fill a list with all the alive connections.
  \param out A list that will receive all the connections alive on the
  server.
 */
void ConnectionsScheduler::getConnections (list<ConnectionPtr> &out)
{
  out.clear ();

  connectionsMutex.lock ();
  try
    {
      HashMap<SocketHandle, ConnectionPtr>::Iterator it = connections.begin ();
      for (; it != connections.end (); it++)
        out.push_back (*it);
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    }
}

/*!
  Get the alive connections number.
 */
u_long ConnectionsScheduler::getNumAliveConnections ()
{
  return connections.size ();
}

/*!
  Remove a connection from the connections set.
 */
void ConnectionsScheduler::removeConnection (ConnectionPtr connection)
{
  connectionsMutex.lock ();
  try
    {
      if (connection->socket)
        connections.remove (connection->socket->getHandle ());
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    }
}

/*!
  Terminate any active connection.
 */
void ConnectionsScheduler::terminateConnections ()
{
  int i;
  connectionsMutex.lock ();
  try
    {
      HashMap<SocketHandle, ConnectionPtr>::Iterator it = connections.begin ();
      for (; it != connections.end (); it++)
        {
          ConnectionPtr c = *it;

          if (c->notifySchedulerHandler (-1))
            continue;

          if (c->allowDelete (true) && c->socket)
            c->socket->close ();
        }
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      throw;
    };

  connections.clear ();

  readyMutex.lock ();
  try
  {
    for (i = 0; i < PRIORITY_CLASSES; i++)
      while (ready[i].size ())
        ready[i].pop ();
    readyMutex.unlock ();
  }
  catch (...)
    {
      readyMutex.unlock ();
      throw;
    }
}

/*!
  Accept a visitor on the connections.
 */
int ConnectionsScheduler::accept (ConnectionsSchedulerVisitor* visitor, void* args)
{
  int ret = 0;
  connectionsMutex.lock ();
  try
    {
      for (HashMap<SocketHandle, ConnectionPtr>::Iterator it =
             connections.begin (); it != connections.end ()  && !ret; it++)
        visitor->visitConnection (*it, args);
      connectionsMutex.unlock ();
    }
  catch (...)
    {
      connectionsMutex.unlock ();
      return 1;
    }
  return ret;
}
