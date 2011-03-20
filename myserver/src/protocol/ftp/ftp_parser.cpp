
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005,
   2006, 2011 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "ftp_parser.ypp"

#include <include/protocol/ftp/ftp.h>
#include <include/protocol/ftp/ftp_parser.h>
#include <include/protocol/ftp/ftp_lexer.h>
#include <include/server/clients_thread.h>
#include <assert.h>


/* Line 189 of yacc.c  */
#line 82 "ftp_parser.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER_FTP_ARG = 258,
     STRING_FTP_ARG = 259,
     CHAR_FTP_ARG = 260,
     CRLF_FTP_SEP = 261,
     SPACE_FTP_SEP = 262,
     PUNCT_FTP_SEP = 263,
     USER_FTP_COMMAND = 264,
     PASS_FTP_COMMAND = 265,
     QUIT_FTP_COMMAND = 266,
     PORT_FTP_COMMAND = 267,
     PASV_FTP_COMMAND = 268,
     MODE_FTP_COMMAND = 269,
     TYPE_FTP_COMMAND = 270,
     STRU_FTP_COMMAND = 271,
     REST_FTP_COMMAND = 272,
     RETR_FTP_COMMAND = 273,
     LIST_FTP_COMMAND = 274,
     NLST_FTP_COMMAND = 275,
     ABOR_FTP_COMMAND = 276,
     CWD_FTP_COMMAND = 277,
     CDUP_FTP_COMMAND = 278,
     PWD_FTP_COMMAND = 279,
     ALLO_FTP_COMMAND = 280,
     STOR_FTP_COMMAND = 281,
     STOU_FTP_COMMAND = 282,
     DELE_FTP_COMMAND = 283,
     APPE_FTP_COMMAND = 284,
     MKD_FTP_COMMAND = 285,
     RMD_FTP_COMMAND = 286,
     RNFR_FTP_COMMAND = 287,
     RNTO_FTP_COMMAND = 288,
     HELP_FTP_COMMAND = 289,
     SYST_FTP_COMMAND = 290,
     STAT_FTP_COMMAND = 291,
     NOOP_FTP_COMMAND = 292,
     SITE_FTP_COMMAND = 293,
     CHMOD_FTP_COMMAND = 294,
     UTIME_FTP_COMMAND = 295,
     SIZE_FTP_COMMAND = 296
   };
#endif
/* Tokens.  */
#define NUMBER_FTP_ARG 258
#define STRING_FTP_ARG 259
#define CHAR_FTP_ARG 260
#define CRLF_FTP_SEP 261
#define SPACE_FTP_SEP 262
#define PUNCT_FTP_SEP 263
#define USER_FTP_COMMAND 264
#define PASS_FTP_COMMAND 265
#define QUIT_FTP_COMMAND 266
#define PORT_FTP_COMMAND 267
#define PASV_FTP_COMMAND 268
#define MODE_FTP_COMMAND 269
#define TYPE_FTP_COMMAND 270
#define STRU_FTP_COMMAND 271
#define REST_FTP_COMMAND 272
#define RETR_FTP_COMMAND 273
#define LIST_FTP_COMMAND 274
#define NLST_FTP_COMMAND 275
#define ABOR_FTP_COMMAND 276
#define CWD_FTP_COMMAND 277
#define CDUP_FTP_COMMAND 278
#define PWD_FTP_COMMAND 279
#define ALLO_FTP_COMMAND 280
#define STOR_FTP_COMMAND 281
#define STOU_FTP_COMMAND 282
#define DELE_FTP_COMMAND 283
#define APPE_FTP_COMMAND 284
#define MKD_FTP_COMMAND 285
#define RMD_FTP_COMMAND 286
#define RNFR_FTP_COMMAND 287
#define RNTO_FTP_COMMAND 288
#define HELP_FTP_COMMAND 289
#define SYST_FTP_COMMAND 290
#define STAT_FTP_COMMAND 291
#define NOOP_FTP_COMMAND 292
#define SITE_FTP_COMMAND 293
#define CHMOD_FTP_COMMAND 294
#define UTIME_FTP_COMMAND 295
#define SIZE_FTP_COMMAND 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 16 "ftp_parser.ypp"

  int		m_nInt;
  char		m_nChar;
  char		m_szStr[PARSER_STR_LEN];
  FtpHost 	m_host;



/* Line 214 of yacc.c  */
#line 209 "ftp_parser.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 23 "ftp_parser.ypp"

#define YYSTACK_ALLOC gnulib::malloc
#define yyscanner pContext->getScanner()


/* Line 264 of yacc.c  */
#line 240 "ftp_parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   217

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  60
/* YYNRULES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    43,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    49,    45,    46,
       2,    51,    47,     2,     2,    50,     2,     2,    52,     2,
      53,     2,    48,     2,    42,    44,    54,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     7,    12,    17,    22,    25,    30,
      35,    42,    47,    52,    57,    66,    71,    74,    77,    82,
      85,    90,    95,    98,   101,   106,   109,   112,   115,   118,
     121,   126,   129,   131,   136,   141,   146,   151,   156,   161,
     166,   171,   175,   182,   189,   194,   198,   206,   210,   212,
     214,   216,   218,   220,   222,   224,   226,   228,   230,   232,
     234
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    -1,    56,    57,    -1,     9,     7,     4,
       6,    -1,    10,     7,     4,     6,    -1,    12,     7,    58,
       6,    -1,    13,     6,    -1,    14,     7,    61,     6,    -1,
      15,     7,    63,     6,    -1,    15,     7,    63,     7,    64,
       6,    -1,    16,     7,    62,     6,    -1,    17,     7,     4,
       6,    -1,    25,     7,     3,     6,    -1,    25,     7,     3,
       7,    42,     7,     3,     6,    -1,    18,     7,     4,     6,
      -1,    21,     6,    -1,    19,     6,    -1,    19,     7,     4,
       6,    -1,    20,     6,    -1,    20,     7,     4,     6,    -1,
      22,     7,     4,     6,    -1,    23,     6,    -1,    24,     6,
      -1,    34,     7,     4,     6,    -1,    34,     6,    -1,    35,
       6,    -1,    36,     6,    -1,    11,     6,    -1,    37,     6,
      -1,     4,     7,     4,     6,    -1,     4,     6,    -1,     6,
      -1,    26,     7,     4,     6,    -1,    27,     7,     4,     6,
      -1,    28,     7,     4,     6,    -1,    29,     7,     4,     6,
      -1,    30,     7,     4,     6,    -1,    31,     7,     4,     6,
      -1,    32,     7,     4,     6,    -1,    33,     7,     4,     6,
      -1,    38,     7,    57,    -1,    39,     7,     4,     7,     4,
       6,    -1,    40,     7,     4,     7,     4,     6,    -1,    41,
       7,     4,     6,    -1,    59,    43,    60,    -1,     3,    43,
       3,    43,     3,    43,     3,    -1,     3,    43,     3,    -1,
      44,    -1,    45,    -1,    46,    -1,    47,    -1,    42,    -1,
      48,    -1,    49,    -1,    50,    -1,    51,    -1,    52,    -1,
      53,    -1,    54,    -1,    46,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    94,    94,    95,   101,   105,   109,   113,   117,   121,
     128,   135,   139,   143,   147,   151,   155,   159,   163,   167,
     171,   175,   179,   183,   187,   191,   195,   199,   203,   207,
     211,   215,   219,   223,   227,   231,   235,   239,   243,   247,
     251,   255,   258,   263,   269,   276,   287,   296,   303,   307,
     311,   317,   321,   325,   331,   335,   339,   343,   349,   353,
     357
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER_FTP_ARG", "STRING_FTP_ARG",
  "CHAR_FTP_ARG", "CRLF_FTP_SEP", "SPACE_FTP_SEP", "PUNCT_FTP_SEP",
  "USER_FTP_COMMAND", "PASS_FTP_COMMAND", "QUIT_FTP_COMMAND",
  "PORT_FTP_COMMAND", "PASV_FTP_COMMAND", "MODE_FTP_COMMAND",
  "TYPE_FTP_COMMAND", "STRU_FTP_COMMAND", "REST_FTP_COMMAND",
  "RETR_FTP_COMMAND", "LIST_FTP_COMMAND", "NLST_FTP_COMMAND",
  "ABOR_FTP_COMMAND", "CWD_FTP_COMMAND", "CDUP_FTP_COMMAND",
  "PWD_FTP_COMMAND", "ALLO_FTP_COMMAND", "STOR_FTP_COMMAND",
  "STOU_FTP_COMMAND", "DELE_FTP_COMMAND", "APPE_FTP_COMMAND",
  "MKD_FTP_COMMAND", "RMD_FTP_COMMAND", "RNFR_FTP_COMMAND",
  "RNTO_FTP_COMMAND", "HELP_FTP_COMMAND", "SYST_FTP_COMMAND",
  "STAT_FTP_COMMAND", "NOOP_FTP_COMMAND", "SITE_FTP_COMMAND",
  "CHMOD_FTP_COMMAND", "UTIME_FTP_COMMAND", "SIZE_FTP_COMMAND", "'r'",
  "','", "'s'", "'b'", "'c'", "'f'", "'p'", "'a'", "'i'", "'e'", "'l'",
  "'n'", "'t'", "$accept", "cmd_list", "cmd", "host_port", "host_number",
  "port_number", "mode_code", "structure_code", "type_code",
  "format_control_code", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   114,    44,   115,    98,    99,   102,   112,    97,
     105,   101,   108,   110,   116
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    58,    59,    60,    61,    61,
      61,    62,    62,    62,    63,    63,    63,    63,    64,    64,
      64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     4,     4,     4,     2,     4,     4,
       6,     4,     4,     4,     8,     4,     2,     2,     4,     2,
       4,     4,     2,     2,     4,     2,     2,     2,     2,     2,
       4,     2,     1,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     6,     6,     4,     3,     7,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    32,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    31,
       0,     0,     0,    28,     0,     7,     0,     0,     0,     0,
       0,    17,     0,    19,     0,    16,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
      26,    27,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,     0,    54,    55,    56,
      57,     0,    52,    51,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,     0,     0,    30,     4,     5,     0,     6,
       0,     8,     9,     0,    11,    12,    15,    18,    20,    21,
      13,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      24,     0,     0,    44,     0,     0,    45,    60,    58,    59,
       0,     0,     0,     0,     0,     0,    10,     0,    42,    43,
       0,    47,     0,     0,    14,    46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    38,    81,    82,   146,    86,    95,    91,   150
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -41
static const yytype_int16 yypact[] =
{
     -41,     0,   -41,    39,   -41,    -6,    -4,    -1,    36,    94,
      95,    96,    97,    98,    99,    79,    86,   101,   102,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      88,   116,   117,   118,   119,   120,   121,   122,   -41,   -41,
     126,   127,   128,   -41,   130,   -41,    45,    32,   -40,   131,
     132,   -41,   133,   -41,   134,   -41,   135,   -41,   -41,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   -41,   146,
     -41,   -41,   -41,    38,   147,   148,   149,   150,   151,   152,
      58,   153,    65,   -41,   -41,   -41,   154,   -41,   -41,   -41,
     -41,    90,   -41,   -41,   -41,   155,   156,   157,   158,   159,
     160,    92,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   -41,   170,   171,   173,   -41,   -41,   -41,   177,   -41,
     178,   -41,   -41,    34,   -41,   -41,   -41,   -41,   -41,   -41,
     -41,    70,   -41,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
     -41,   172,   179,   -41,    82,    91,   -41,   -41,   -41,   -41,
     176,   180,   182,   183,   181,   187,   -41,   188,   -41,   -41,
     174,   -41,   186,   190,   -41,   -41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,    68,   -41,   -41,   -41,   -41,   -41,   -41,   -41
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       2,    41,    92,    42,     3,    43,     4,    93,    94,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,     3,    44,     4,    39,    40,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
     147,    87,    88,    89,    90,    51,    52,   148,   149,    83,
      84,    85,    53,    54,    68,    69,   122,   123,   130,   131,
      45,   118,    46,    47,    48,    49,    50,    55,   120,    56,
      57,    58,   151,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    70,    71,    72,   154,    73,    74,    75,    76,
      77,    78,    79,    80,   155,    96,    97,    98,    99,   100,
     101,   111,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   112,   113,   114,     0,     0,   115,   116,   117,   119,
     121,   124,   125,   126,   127,   128,   129,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   152,   141,   142,   143,
     144,   145,   156,   153,   160,     0,     0,   157,   158,   159,
     161,   162,   164,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163
};

static const yytype_int8 yycheck[] =
{
       0,     7,    42,     7,     4,     6,     6,    47,    48,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,     4,     7,     6,     6,     7,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      46,    49,    50,    51,    52,     6,     7,    53,    54,    44,
      45,    46,     6,     7,     6,     7,     6,     7,     6,     7,
       6,    43,     7,     7,     7,     7,     7,     6,    43,     7,
       6,     6,    42,     7,     7,     7,     7,     7,     7,     7,
       7,     7,     6,     6,     6,    43,     7,     7,     7,     7,
       4,     4,     4,     3,    43,     4,     4,     4,     4,     4,
       3,    73,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,    -1,    -1,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     4,     7,     7,     6,
       3,     3,     6,     4,     3,    -1,    -1,     7,     6,     6,
       3,     3,     6,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,     0,     4,     6,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    57,     6,
       7,     7,     7,     6,     7,     6,     7,     7,     7,     7,
       7,     6,     7,     6,     7,     6,     7,     6,     6,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     6,     7,
       6,     6,     6,     7,     7,     7,     7,     4,     4,     4,
       3,    58,    59,    44,    45,    46,    61,    49,    50,    51,
      52,    63,    42,    47,    48,    62,     4,     4,     4,     4,
       4,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,    57,     4,     4,     4,     6,     6,     6,    43,     6,
      43,     6,     6,     7,     6,     6,     6,     6,     6,     6,
       6,     7,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     7,     7,     6,     3,     3,    60,    46,    53,    54,
      64,    42,     4,     4,    43,    43,     6,     7,     6,     6,
       3,     3,     3,    43,     6,     3
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, pContext, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, pContext); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Ftp *pContext)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, pContext)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Ftp *pContext;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (pContext);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Ftp *pContext)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, pContext)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Ftp *pContext;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, pContext);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Ftp *pContext)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, pContext)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Ftp *pContext;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , pContext);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, pContext); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Ftp *pContext)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, pContext)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Ftp *pContext;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (pContext);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (Ftp *pContext);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (Ftp *pContext)
#else
int
yyparse (pContext)
    Ftp *pContext;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1455 of yacc.c  */
#line 96 "ftp_parser.ypp"
    {
			pContext->computeParseLength((yyloc));
		}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 102 "ftp_parser.ypp"
    {
			pContext->user((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 106 "ftp_parser.ypp"
    {
			pContext->password((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 110 "ftp_parser.ypp"
    {
		  pContext->port((yyvsp[(3) - (4)].m_host));
		}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 114 "ftp_parser.ypp"
    {
		  pContext->pasv();
		}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 118 "ftp_parser.ypp"
    {
		  pContext->mode((yyvsp[(3) - (4)].m_nInt));
		}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 122 "ftp_parser.ypp"
    {
      if ((yyvsp[(3) - (4)].m_nInt) == -1)
        pContext->ftpReply(504);
      else
        pContext->type((yyvsp[(3) - (4)].m_nInt));
		}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 129 "ftp_parser.ypp"
    {
      if ((yyvsp[(3) - (6)].m_nInt) == -1 || (yyvsp[(5) - (6)].m_nInt) == -1)
        pContext->ftpReply(504);
      else
        pContext->type((yyvsp[(3) - (6)].m_nInt), (yyvsp[(5) - (6)].m_nInt));
		}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 136 "ftp_parser.ypp"
    {
		  pContext->stru((yyvsp[(3) - (4)].m_nInt));
		}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 140 "ftp_parser.ypp"
    {
		  pContext->rest((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 144 "ftp_parser.ypp"
    {
		  pContext->allo((yyvsp[(3) - (4)].m_nInt));
		}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 148 "ftp_parser.ypp"
    {
		  pContext->allo((yyvsp[(3) - (8)].m_nInt), (yyvsp[(7) - (8)].m_nInt));
		}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 152 "ftp_parser.ypp"
    {
		  pContext->retr((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 156 "ftp_parser.ypp"
    {
			pContext->abor();
		}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 160 "ftp_parser.ypp"
    {
		  pContext->list();
		}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 164 "ftp_parser.ypp"
    {
		  pContext->list((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 168 "ftp_parser.ypp"
    {
		  pContext->nlst();
		}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 172 "ftp_parser.ypp"
    {
		  pContext->nlst((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 176 "ftp_parser.ypp"
    {
			pContext->cwd((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 180 "ftp_parser.ypp"
    {
			pContext->cwd("..");
		}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 184 "ftp_parser.ypp"
    {
			pContext->pwd();
		}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 188 "ftp_parser.ypp"
    {
			pContext->help((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 192 "ftp_parser.ypp"
    {
			pContext->help();
		}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 196 "ftp_parser.ypp"
    {
			pContext->syst();
		}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 200 "ftp_parser.ypp"
    {
			pContext->statCmd();
		}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 204 "ftp_parser.ypp"
    {
			pContext->quit();
		}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 208 "ftp_parser.ypp"
    {
			pContext->noop();
		}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 212 "ftp_parser.ypp"
    {
			pContext->ftpReply(502);//any new commands
		}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 216 "ftp_parser.ypp"
    {
			pContext->ftpReply(502);//any new commands
		}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 220 "ftp_parser.ypp"
    {
			// ignore empty lines
		}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 224 "ftp_parser.ypp"
    {
		  pContext->stor((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 228 "ftp_parser.ypp"
    {
		  pContext->stou((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 232 "ftp_parser.ypp"
    {
		  pContext->dele((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 236 "ftp_parser.ypp"
    {
		  pContext->appe((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 240 "ftp_parser.ypp"
    {
		  pContext->mkd((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 244 "ftp_parser.ypp"
    {
		  pContext->rmd((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 248 "ftp_parser.ypp"
    {
		  pContext->rnfr((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 252 "ftp_parser.ypp"
    {
		  pContext->Rnto((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 256 "ftp_parser.ypp"
    {
		}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 259 "ftp_parser.ypp"
    {
			//pContext->Chmod($3, $5);
			pContext->ftpReply(502);
		}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 264 "ftp_parser.ypp"
    {
			//pContext->Utime($3, $5);
			pContext->ftpReply(502);
		}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 270 "ftp_parser.ypp"
    {
			pContext->size((yyvsp[(3) - (4)].m_szStr));
		}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 277 "ftp_parser.ypp"
    {
		  (yyval.m_host).h1 = (yyvsp[(1) - (3)].m_host).h1;
		  (yyval.m_host).h2 = (yyvsp[(1) - (3)].m_host).h2;
		  (yyval.m_host).h3 = (yyvsp[(1) - (3)].m_host).h3;
		  (yyval.m_host).h4 = (yyvsp[(1) - (3)].m_host).h4;
		  (yyval.m_host).p1 = (yyvsp[(3) - (3)].m_host).p1;
		  (yyval.m_host).p2 = (yyvsp[(3) - (3)].m_host).p2;
		}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 288 "ftp_parser.ypp"
    {
		  (yyval.m_host).h1 = (yyvsp[(1) - (7)].m_nInt);
		  (yyval.m_host).h2 = (yyvsp[(3) - (7)].m_nInt);
		  (yyval.m_host).h3 = (yyvsp[(5) - (7)].m_nInt);
		  (yyval.m_host).h4 = (yyvsp[(7) - (7)].m_nInt);
		}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 297 "ftp_parser.ypp"
    {
		  (yyval.m_host).p1 = (yyvsp[(1) - (3)].m_nInt);
		  (yyval.m_host).p2 = (yyvsp[(3) - (3)].m_nInt);
		}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 304 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = FtpuserData::MODE_STREAM;
		}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 308 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = -1;
		}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 312 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = -1;
		}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 318 "ftp_parser.ypp"
    {
			(yyval.m_nInt) = FtpuserData::STRU_FILE;
		}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 322 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = -1;
		}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 326 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = -1;
		}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 332 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = FtpuserData::REPR_ASCII;
		}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 336 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = FtpuserData::REPR_IMAGE;
		}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 340 "ftp_parser.ypp"
    {
      (yyval.m_nInt) = -1;
		}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 344 "ftp_parser.ypp"
    {
      (yyval.m_nInt) = -1;
		}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 350 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = FtpuserData::NON_PRINT;
		}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 354 "ftp_parser.ypp"
    {
		  (yyval.m_nInt) = -1;
		}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 358 "ftp_parser.ypp"
    {
      (yyval.m_nInt) = -1;
		}
    break;



/* Line 1455 of yacc.c  */
#line 2158 "ftp_parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, pContext, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, pContext, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, pContext, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, pContext);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, pContext);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, pContext, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, pContext);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, pContext);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 362 "ftp_parser.ypp"


int Ftp::parseControlConnection()
{
  if (td.buffer == NULL || td.buffer->getBuffer() == NULL)
    return ClientsThread::DELETE_CONNECTION;

  FtpuserData *pFtpuserData = static_cast<FtpuserData *> (td.pConnection->protocolBuffer);
  if (pFtpuserData == NULL)
    return ClientsThread::DELETE_CONNECTION;

  /* awaiting commands reply.  */
  if (pFtpuserData->m_nFtpstate <= FtpuserData::NO_CONTROL_CONNECTION)
    {
      ftpReply (220);
      pFtpuserData->m_nFtpstate = FtpuserData::CONTROL_CONNECTION_UP;
    }

  MemBuf out;
  escapeTelnet (*td.buffer, out);
  if (! m_bEnablePipelining)
    {
      MemBuf tmpBuff (out);
      removePipelinedCmds (tmpBuff, out);
    }
  char *pBuffToParse = out.getBuffer ();

  if (pBuffToParse == NULL || td.buffersize <= td.m_nParseLength )
    return ClientsThread::KEEP_CONNECTION;
  else
    pBuffToParse += td.m_nParseLength;

  yylex_init (&m_scanner);
  yy_buffer_state *bufstate = yy_scan_string (pBuffToParse, m_scanner);
  int nParserRet = yyparse (this);
  yy_delete_buffer (bufstate, m_scanner);
  yylex_destroy (m_scanner);

  switch (nParserRet)
    {
    case 0:
      if (pFtpuserData->m_nFtpstate == FtpuserData::NO_CONTROL_CONNECTION )
        return ClientsThread::DELETE_CONNECTION;
      else
        return ClientsThread::KEEP_CONNECTION;

    case 1:
      /* Invalid input.  */
      return ClientsThread::DELETE_CONNECTION;

    case 2:
      /* memory exhaustion!  */
      return ClientsThread::DELETE_CONNECTION;
    }
  /*TODO: handle INCOMPLETE_REQUEST */
  return ClientsThread::DELETE_CONNECTION;
}

u_long Ftp::computeParseLength (const YYLTYPE &location)
{
  if (td.buffer == NULL)
    return 1;

  int nBuffLine = 1, nBuffCol = 0;
  char *pszBuff = td.buffer->getBuffer ();
  for (u_long i = 0; i < td.buffersize; i++)
    {
      if (pszBuff[i] == '\n')
        {
          nBuffLine++;
          nBuffCol = 0;
        }
      else
        nBuffCol++;
      if (nBuffLine == location.last_line && nBuffCol == location.last_column)
        {
          td.m_nParseLength = i;
          return 0;
        }
    }

  return 1;
}

/* Global functions.  */

void yyerror (YYLTYPE *pLoc, Ftp *pContext, const char *msg)
{
  if (pContext == NULL)
    return;
  pContext->printError (msg);
}

