/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --language=ANSI-C --struct-type --slot-name=name --hash-fn-name=java_hash --lookup-fn-name=java_lookup  */
/* Computed positions: -k'1,3' */

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

#define JAVA_ABSTRACT	2001
#define JAVA_BOOLEAN	2002
#define JAVA_BREAK	2003
#define JAVA_BYTE	2004
#define JAVA_CASE	2005
#define JAVA_CATCH	2006
#define JAVA_CHAR	2007
#define JAVA_CLASS	2008
#define JAVA_CONST	2009
#define JAVA_CONTINUE	2010
#define JAVA_DEFAULT	2011
#define JAVA_DO	2012
#define JAVA_DOUBLE	2013
#define JAVA_ELSE	2014
#define JAVA_ENUM	2015
#define JAVA_EXTENDS	2016
#define JAVA_FALSE	2017
#define JAVA_FINAL	2018
#define JAVA_FINALLY	2019
#define JAVA_FLOAT	2020
#define JAVA_FOR	2021
#define JAVA_GOTO	2022
#define JAVA_IF	2023
#define JAVA_IMPLEMENTS	2024
#define JAVA_IMPORT	2025
#define JAVA_INSTANCEOF	2026
#define JAVA_INT	2027
#define JAVA_INTERFACE	2028
#define JAVA_LONG	2029
#define JAVA_NATIVE	2030
#define JAVA_NEW	2031
#define JAVA_NULL	2032
#define JAVA_PACKAGE	2033
#define JAVA_PRIVATE	2034
#define JAVA_PROTECTED	2035
#define JAVA_PUBLIC	2036
#define JAVA_RECORD	2037
#define JAVA_RETURN	2038
#define JAVA_SHORT	2039
#define JAVA_STATIC	2040
#define JAVA_STRICTFP	2041
#define JAVA_SUPER	2042
#define JAVA_SWITCH	2043
#define JAVA_SYNCHRONIZED	2044
#define JAVA_THIS	2045
#define JAVA_THROW	2046
#define JAVA_THROWS	2047
#define JAVA_UNION	2048
#define JAVA_TRANSIENT	2049
#define JAVA_TRUE	2050
#define JAVA_TRY	2051
#define JAVA_VOID	2052
#define JAVA_VOLATILE	2053
#define JAVA_WHILE	2054
#define JAVA_WIDEFP	2055
struct keyword { char *name; int token; };

#define TOTAL_KEYWORDS 55
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 112
/* maximum key range = 111, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
java_hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113,  20,   5,  15,
        0,  60,  35,  35, 113,  15,  25, 113,   0,  45,
       30,  10,   0,  55,  10,   0,  30,  45,   0,  40,
       15, 113,  10, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113, 113, 113, 113,
      113, 113, 113, 113, 113, 113, 113
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[2]+1];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct keyword *
java_lookup (register const char *str, register size_t len)
{
  static struct keyword wordlist[] =
    {
      {""}, {""},
      {"do", JAVA_DO},
      {""}, {""},
      {"short", JAVA_SHORT},
      {"double", JAVA_DOUBLE},
      {"package", JAVA_PACKAGE},
      {"strictfp", JAVA_STRICTFP},
      {"protected", JAVA_PROTECTED},
      {""},
      {"static", JAVA_STATIC},
      {"boolean", JAVA_BOOLEAN},
      {""},
      {"long", JAVA_LONG},
      {""},
      {"record", JAVA_RECORD},
      {"if", JAVA_IF},
      {""}, {""}, {""},
      {"public", JAVA_PUBLIC},
      {"synchronized", JAVA_SYNCHRONIZED},
      {""},
      {"char", JAVA_CHAR},
      {"class", JAVA_CLASS},
      {""}, {""}, {""},
      {"void", JAVA_VOID},
      {"const", JAVA_CONST},
      {"switch", JAVA_SWITCH},
      {"private", JAVA_PRIVATE},
      {"continue", JAVA_CONTINUE},
      {"true", JAVA_TRUE},
      {"throw", JAVA_THROW},
      {"throws", JAVA_THROWS},
      {""},
      {"for", JAVA_FOR},
      {""},
      {"float", JAVA_FLOAT},
      {""},
      {"default", JAVA_DEFAULT},
      {"try", JAVA_TRY},
      {"transient", JAVA_TRANSIENT},
      {"break", JAVA_BREAK},
      {""}, {""},
      {"new", JAVA_NEW},
      {"case", JAVA_CASE},
      {"final", JAVA_FINAL},
      {""},
      {"finally", JAVA_FINALLY},
      {"volatile", JAVA_VOLATILE},
      {"byte", JAVA_BYTE},
      {"instanceof", JAVA_INSTANCEOF},
      {""}, {""},
      {"abstract", JAVA_ABSTRACT},
      {"this", JAVA_THIS},
      {"super", JAVA_SUPER},
      {"return", JAVA_RETURN},
      {""},
      {"int", JAVA_INT},
      {"enum", JAVA_ENUM},
      {"catch", JAVA_CATCH},
      {""}, {""}, {""},
      {"interface", JAVA_INTERFACE},
      {"while", JAVA_WHILE},
      {""}, {""}, {""}, {""},
      {"union", JAVA_UNION},
      {"import", JAVA_IMPORT},
      {""}, {""},
      {"null", JAVA_NULL},
      {"implements", JAVA_IMPLEMENTS},
      {"native", JAVA_NATIVE},
      {""}, {""},
      {"goto", JAVA_GOTO},
      {"false", JAVA_FALSE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"else", JAVA_ELSE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
      {"widefp", JAVA_WIDEFP},
      {""}, {""}, {""}, {""}, {""},
      {"extends", JAVA_EXTENDS}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = java_hash (str, len);

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
java_reserved_word(const char *str, int len)
{
	struct keyword *keyword;

	keyword = java_lookup(str, len);
	return (keyword && IS_RESERVED_WORD(keyword->token)) ? keyword->token : 0;
}
