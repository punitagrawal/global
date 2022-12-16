/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --language=ANSI-C --struct-type --slot-name=name --hash-fn-name=cpp_hash --lookup-fn-name=cpp_lookup  */
/* Computed positions: -k'2-4' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif


#include "strmake.h"
#define START_VARIABLE	1001
#define START_WORD	2001
#define START_SHARP	3001
#define START_YACC	4001
#define IS_RESERVED_WORD(a)	((a) >= START_WORD)
#define IS_RESERVED_VARIABLE(a)	((a) >= START_VARIABLE && (a) < START_WORD)
#define IS_RESERVED_SHARP(a)	((a) >= START_SHARP && (a) < START_YACC)
#define IS_RESERVED_YACC(a)	((a) >= START_YACC)

#define CPP_WCOLON	2001
#define CPP___P	2002
#define CPP___ATTRIBUTE__	2003
#define CPP___EXTENSION__	2004
#define CPP___THREAD	2005
#define CPP_ASM	2006
#define CPP_CONST	2007
#define CPP_INLINE	2008
#define CPP_SIGNED	2009
#define CPP_VOLATILE	2010
#define CPP_AUTO	2011
#define CPP_BOOL	2012
#define CPP_BREAK	2013
#define CPP_CASE	2014
#define CPP_CATCH	2015
#define CPP_CHAR	2016
#define CPP_CLASS	2017
#define CPP_CONST_CAST	2018
#define CPP_CONTINUE	2019
#define CPP_DEFAULT	2020
#define CPP_DELETE	2021
#define CPP_DO	2022
#define CPP_DOUBLE	2023
#define CPP_DYNAMIC_CAST	2024
#define CPP_ELSE	2025
#define CPP_ENUM	2026
#define CPP_EXPLICIT	2027
#define CPP_EXPORT	2028
#define CPP_EXTERN	2029
#define CPP_FALSE	2030
#define CPP_FINAL	2031
#define CPP_FLOAT	2032
#define CPP_FOR	2033
#define CPP_FRIEND	2034
#define CPP_GOTO	2035
#define CPP_IF	2036
#define CPP_INT	2037
#define CPP_LONG	2038
#define CPP_MUTABLE	2039
#define CPP_NAMESPACE	2040
#define CPP_NEW	2041
#define CPP_OPERATOR	2042
#define CPP_PRIVATE	2043
#define CPP_PROTECTED	2044
#define CPP_PUBLIC	2045
#define CPP_REGISTER	2046
#define CPP_REINTERPRET_CAST	2047
#define CPP_RETURN	2048
#define CPP_SHORT	2049
#define CPP_SIZEOF	2050
#define CPP_STATIC	2051
#define CPP_STATIC_CAST	2052
#define CPP_STRUCT	2053
#define CPP_SWITCH	2054
#define CPP_TEMPLATE	2055
#define CPP_THIS	2056
#define CPP_THROW	2057
#define CPP_TRUE	2058
#define CPP_TRY	2059
#define CPP_TYPEDEF	2060
#define CPP_TYPENAME	2061
#define CPP_TYPEID	2062
#define CPP_UNION	2063
#define CPP_UNSIGNED	2064
#define CPP_USING	2065
#define CPP_VIRTUAL	2066
#define CPP_VOID	2067
#define CPP_WCHAR_T	2068
#define CPP_WHILE	2069
#define SHARP_SHARP	3001
#define SHARP_ASSERT	3002
#define SHARP_DEFINE	3003
#define SHARP_ELIF	3004
#define SHARP_ELSE	3005
#define SHARP_ENDIF	3006
#define SHARP_ERROR	3007
#define SHARP_IDENT	3008
#define SHARP_IF	3009
#define SHARP_IFDEF	3010
#define SHARP_IFNDEF	3011
#define SHARP_IMPORT	3012
#define SHARP_INCLUDE	3013
#define SHARP_INCLUDE_NEXT	3014
#define SHARP_LINE	3015
#define SHARP_PRAGMA	3016
#define SHARP_SCCS	3017
#define SHARP_UNASSERT	3018
#define SHARP_UNDEF	3019
#define SHARP_WARNING	3020
struct keyword { char *name; int token; };

#define TOTAL_KEYWORDS 100
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 16
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 227
/* maximum key range = 226, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
cpp_hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228,  10, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228,   0, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228,  45, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228,   5, 228,  10,  15,  80,
       40,  25,  90,  80,  80,  15,  70, 228,  25,  25,
        0,   5,  55,  15,   5,   0,   0,  10, 100, 110,
       15,  20,  30,  40, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228, 228, 228, 228,
      228, 228, 228, 228, 228, 228, 228
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]+1];
      /*FALLTHROUGH*/
      case 2:
        hval += asso_values[(unsigned char)str[1]];
        break;
    }
  return hval;
}

struct keyword *
cpp_lookup (register const char *str, register size_t len)
{
  static struct keyword wordlist[] =
    {
      {""}, {""},
      {"::", CPP_WCOLON},
      {"asm", CPP_ASM},
      {""}, {""}, {""},
      {"do", CPP_DO},
      {"for", CPP_FOR},
      {""}, {""}, {""},
      {"##", SHARP_SHARP},
      {"int", CPP_INT},
      {""},
      {"const", CPP_CONST},
      {"struct", CPP_STRUCT},
      {"#assert", SHARP_ASSERT},
      {"continue", CPP_CONTINUE},
      {""},
      {"const_cast", CPP_CONST_CAST},
      {"static", CPP_STATIC},
      {"virtual", CPP_VIRTUAL},
      {"unsigned", CPP_UNSIGNED},
      {"goto", CPP_GOTO},
      {"__asm", CPP_ASM},
      {"static_cast", CPP_STATIC_CAST},
      {"__asm__", CPP_ASM},
      {"__signed", CPP_SIGNED},
      {"auto", CPP_AUTO},
      {"__signed__", CPP_SIGNED},
      {"__attribute", CPP___ATTRIBUTE__},
      {""},
      {"__attribute__", CPP___ATTRIBUTE__},
      {"#unassert", SHARP_UNASSERT},
      {"final", CPP_FINAL},
      {"#error", SHARP_ERROR},
      {"mutable", CPP_MUTABLE},
      {"try", CPP_TRY},
      {"case", CPP_CASE},
      {"false", CPP_FALSE},
      {"export", CPP_EXPORT},
      {""},
      {"new", CPP_NEW},
      {"namespace", CPP_NAMESPACE},
      {"class", CPP_CLASS},
      {"inline", CPP_INLINE},
      {"dynamic_cast", CPP_DYNAMIC_CAST},
      {"volatile", CPP_VOLATILE},
      {""}, {""},
      {"return", CPP_RETURN},
      {""},
      {"__P", CPP___P},
      {"else", CPP_ELSE},
      {"#else", SHARP_ELSE},
      {"extern", CPP_EXTERN},
      {"__const", CPP_CONST},
      {""},
      {"__const__", CPP_CONST},
      {""},
      {"#undef", SHARP_UNDEF},
      {""},
      {"explicit", CPP_EXPLICIT},
      {""}, {""},
      {"typeid", CPP_TYPEID},
      {"typedef", CPP_TYPEDEF},
      {"typename", CPP_TYPENAME},
      {"protected", CPP_PROTECTED},
      {"#elif", SHARP_ELIF},
      {"#ident", SHARP_IDENT},
      {"#pragma", SHARP_PRAGMA},
      {""}, {""},
      {"using", CPP_USING},
      {"#endif", SHARP_ENDIF},
      {"#import", SHARP_IMPORT},
      {""}, {""},
      {"union", CPP_UNION},
      {"delete", CPP_DELETE},
      {""},
      {"__inline", CPP_INLINE},
      {""},
      {"__inline__", CPP_INLINE},
      {"sizeof", CPP_SIZEOF},
      {""},
      {"template", CPP_TEMPLATE},
      {"bool", CPP_BOOL},
      {"throw", CPP_THROW},
      {""},
      {"if", CPP_IF},
      {""},
      {"long", CPP_LONG},
      {"float", CPP_FLOAT},
      {""}, {""},
      {"#if", SHARP_IF},
      {""},
      {"#line", SHARP_LINE},
      {"signed", CPP_SIGNED},
      {"#ifndef", SHARP_IFNDEF},
      {"__thread", CPP___THREAD},
      {"char", CPP_CHAR},
      {"catch", CPP_CATCH},
      {"friend", CPP_FRIEND},
      {""},
      {"#include", SHARP_INCLUDE},
      {""},
      {"break", CPP_BREAK},
      {"reinterpret_cast", CPP_REINTERPRET_CAST},
      {"wchar_t", CPP_WCHAR_T},
      {"#include_next", SHARP_INCLUDE_NEXT},
      {""}, {""}, {""}, {""}, {""},
      {"void", CPP_VOID},
      {""},
      {"public", CPP_PUBLIC},
      {"default", CPP_DEFAULT},
      {"__extension__", CPP___EXTENSION__},
      {""},
      {"#sccs", SHARP_SCCS},
      {"double", CPP_DOUBLE},
      {""},
      {"register", CPP_REGISTER},
      {"enum", CPP_ENUM},
      {"__volatile", CPP_VOLATILE},
      {""},
      {"__volatile__", CPP_VOLATILE},
      {""},
      {"true", CPP_TRUE},
      {""}, {""}, {""},
      {"#warning", SHARP_WARNING},
      {""}, {""},
      {"#ifdef", SHARP_IFDEF},
      {""}, {""}, {""},
      {"short", CPP_SHORT},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"this", CPP_THIS},
      {""}, {""}, {""},
      {"operator", CPP_OPERATOR},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
      {"while", CPP_WHILE},
      {""},
      {"private", CPP_PRIVATE},
      {""}, {""}, {""},
      {"switch", CPP_SWITCH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
      {"#define", SHARP_DEFINE}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = cpp_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

int
cpp_reserved_word(const char *str, int len)
{
	struct keyword *keyword;

	keyword = cpp_lookup(str, len);
	return (keyword && IS_RESERVED_WORD(keyword->token)) ? keyword->token : 0;
}
int
cpp_reserved_sharp(const char *str, int len)
{
	struct keyword *keyword;

	/* Delete blanks. Ex. ' # define ' => '#define' */
	str = strtrim(str, TRIM_ALL, &len);

	keyword = cpp_lookup(str, len);
	return (keyword && IS_RESERVED_SHARP(keyword->token)) ? keyword->token : 0;
}
