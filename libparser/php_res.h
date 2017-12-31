/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --language=ANSI-C --struct-type --slot-name=name --hash-fn-name=php_hash --lookup-fn-name=php_lookup  */
/* Computed positions: -k'1-2,4-5,8-9' */

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

#define PHP___CLASS__	2001
#define PHP___DIR__	2002
#define PHP___FILE__	2003
#define PHP___FUNCTION__	2004
#define PHP___LINE__	2005
#define PHP___METHOD__	2006
#define PHP___NAMESPACE__	2007
#define PHP___TRAIT__	2008
#define PHP_ABSTRACT	2009
#define PHP_AND	2010
#define PHP_ARRAY	2011
#define PHP_AS	2012
#define PHP_BREAK	2013
#define PHP_CALLABLE	2014
#define PHP_CASE	2015
#define PHP_CATCH	2016
#define PHP_CFUNCTION	2017
#define PHP_CLASS	2018
#define PHP_CLONE	2019
#define PHP_CONST	2020
#define PHP_CONTINUE	2021
#define PHP_DECLARE	2022
#define PHP_DEFAULT	2023
#define PHP_DEFINE	2024
#define PHP_DIE	2025
#define PHP_DO	2026
#define PHP_EACH	2027
#define PHP_ECHO	2028
#define PHP_ELSE	2029
#define PHP_ELSEIF	2030
#define PHP_EMPTY	2031
#define PHP_ENDDECLARE	2032
#define PHP_ENDFOR	2033
#define PHP_ENDFOREACH	2034
#define PHP_ENDIF	2035
#define PHP_ENDSWITCH	2036
#define PHP_ENDWHILE	2037
#define PHP_EVAL	2038
#define PHP_EXIT	2039
#define PHP_EXTENDS	2040
#define PHP_FALSE	2041
#define PHP_FINAL	2042
#define PHP_FINALLY	2043
#define PHP_FOR	2044
#define PHP_FOREACH	2045
#define PHP_FUNCTION	2046
#define PHP_GLOBAL	2047
#define PHP_GOTO	2048
#define PHP_IF	2049
#define PHP_IMPLEMENTS	2050
#define PHP_INCLUDE	2051
#define PHP_INCLUDE_ONCE	2052
#define PHP_INSTANCEOF	2053
#define PHP_INSTEADOF	2054
#define PHP_INTERFACE	2055
#define PHP_ISSET	2056
#define PHP_LIST	2057
#define PHP_NAMESPACE	2058
#define PHP_NEW	2059
#define PHP_OLD_FUNCTION	2060
#define PHP_OR	2061
#define PHP_PRINT	2062
#define PHP_PRIVATE	2063
#define PHP_PROTECTED	2064
#define PHP_PUBLIC	2065
#define PHP_REQUIRE	2066
#define PHP_REQUIRE_ONCE	2067
#define PHP_RETURN	2068
#define PHP_STATIC	2069
#define PHP_SWITCH	2070
#define PHP_THROW	2071
#define PHP_TRAIT	2072
#define PHP_TRY	2073
#define PHP_TRUE	2074
#define PHP_UNSET	2075
#define PHP_USE	2076
#define PHP_VAR	2077
#define PHP_WHILE	2078
#define PHP_XOR	2079
#define PHP_YIELD	2080
#define PHP_GLOBALS	1001
#define PHP_HTTP_COOKIE_VARS	1002
#define PHP_HTTP_ENV_VARS	1003
#define PHP_HTTP_GET_VARS	1004
#define PHP_HTTP_POST_FILES	1005
#define PHP_HTTP_POST_VARS	1006
#define PHP_HTTP_SERVER_VARS	1007
#define PHP_HTTP_SESSION_VARS	1008
#define PHP__COOKIE	1009
#define PHP__ENV	1010
#define PHP__FILES	1011
#define PHP__GET	1012
#define PHP__POST	1013
#define PHP__REQUEST	1014
#define PHP__SERVER	1015
#define PHP__SESSION	1016
struct keyword { char *name; int token; };

#define TOTAL_KEYWORDS 254
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 17
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 649
/* maximum key range = 647, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
php_hash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650,  45,  20, 105, 220,  55,
       10,  65,  25,  55, 650,  15,  70,  10,  40,  55,
       90,  10, 120, 120, 100, 110,  70, 240, 110,  80,
      650, 650, 650, 650, 650,  35, 650,   5,  60,   0,
      140,   0,   0,   0,  10,   5, 650,  95,   0, 125,
        0,  20,  75, 650,  55,  40,   5,  70, 165, 230,
      230,  90, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650, 650, 650, 650, 650,
      650, 650, 650, 650, 650, 650
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct keyword *
php_lookup (register const char *str, register size_t len)
{
  static struct keyword wordlist[] =
    {
      {""}, {""}, {""},
      {"new", PHP_NEW},
      {"else", PHP_ELSE},
      {"clone", PHP_CLONE},
      {""},
      {"if", PHP_IF},
      {"and", PHP_AND},
      {"case", PHP_CASE},
      {"endif", PHP_ENDIF},
      {"elseif", PHP_ELSEIF},
      {""}, {""},
      {"list", PHP_LIST},
      {"final", PHP_FINAL},
      {""},
      {"finally", PHP_FINALLY},
      {"callable", PHP_CALLABLE},
      {"each", PHP_EACH},
      {"catch", PHP_CATCH},
      {""}, {""},
      {"for", PHP_FOR},
      {"echo", PHP_ECHO},
      {"Final", PHP_FINAL},
      {"endfor", PHP_ENDFOR},
      {"Finally", PHP_FINALLY},
      {""},
      {"cfunction", PHP_CFUNCTION},
      {""}, {""},
      {"foreach", PHP_FOREACH},
      {"For", PHP_FOR},
      {""},
      {"endforeach", PHP_ENDFOREACH},
      {""}, {""},
      {"continue", PHP_CONTINUE},
      {"insteadof", PHP_INSTEADOF},
      {""}, {""},
      {"Foreach", PHP_FOREACH},
      {"New", PHP_NEW},
      {"goto", PHP_GOTO},
      {"instanceof", PHP_INSTANCEOF},
      {""},
      {"as", PHP_AS},
      {"And", PHP_AND},
      {""},
      {"false", PHP_FALSE},
      {""}, {""}, {""},
      {"namespace", PHP_NAMESPACE},
      {"isset", PHP_ISSET},
      {""},
      {"If", PHP_IF},
      {""},
      {"Else", PHP_ELSE},
      {"False", PHP_FALSE},
      {"static", PHP_STATIC},
      {""},
      {"try", PHP_TRY},
      {"true", PHP_TRUE},
      {"Endif", PHP_ENDIF},
      {"Elseif", PHP_ELSEIF},
      {"IF", PHP_IF},
      {"FOR", PHP_FOR},
      {"interface", PHP_INTERFACE},
      {"const", PHP_CONST},
      {"global", PHP_GLOBAL},
      {"old_function", PHP_OLD_FUNCTION},
      {""},
      {"Each", PHP_EACH},
      {"trait", PHP_TRAIT},
      {""},
      {"or", PHP_OR},
      {"Var", PHP_VAR},
      {"Echo", PHP_ECHO},
      {"unset", PHP_UNSET},
      {"Endfor", PHP_ENDFOR},
      {"include", PHP_INCLUDE},
      {"function", PHP_FUNCTION},
      {"List", PHP_LIST},
      {"class", PHP_CLASS},
      {""},
      {"As", PHP_AS},
      {"AND", PHP_AND},
      {"Insteadof", PHP_INSTEADOF},
      {"Endforeach", PHP_ENDFOREACH},
      {"EndFor", PHP_ENDFOR},
      {""},
      {"Function", PHP_FUNCTION},
      {"Namespace", PHP_NAMESPACE},
      {"Instanceof", PHP_INSTANCEOF},
      {""},
      {"ForEach", PHP_FOREACH},
      {"NEW", PHP_NEW},
      {""},
      {"EndForeach", PHP_ENDFOREACH},
      {""}, {""}, {""}, {""},
      {"Isset", PHP_ISSET},
      {""},
      {"Old_function", PHP_OLD_FUNCTION},
      {""},
      {"Goto", PHP_GOTO},
      {"Clone", PHP_CLONE},
      {""},
      {"Or", PHP_OR},
      {"use", PHP_USE},
      {"Case", PHP_CASE},
      {"EndIf", PHP_ENDIF},
      {"ElseIf", PHP_ELSEIF},
      {"Old_Function", PHP_OLD_FUNCTION},
      {"VAR", PHP_VAR},
      {"Interface", PHP_INTERFACE},
      {"Unset", PHP_UNSET},
      {""}, {""},
      {"Callable", PHP_CALLABLE},
      {"InsteadOf", PHP_INSTEADOF},
      {"Catch", PHP_CATCH},
      {""}, {""}, {""},
      {"EACH", PHP_EACH},
      {"InstanceOf", PHP_INSTANCEOF},
      {""},
      {"Include", PHP_INCLUDE},
      {"Xor", PHP_XOR},
      {"Cfunction", PHP_CFUNCTION},
      {""},
      {"Global", PHP_GLOBAL},
      {"require", PHP_REQUIRE},
      {"abstract", PHP_ABSTRACT},
      {""},
      {"print", PHP_PRINT},
      {"Static", PHP_STATIC},
      {"include_once", PHP_INCLUDE_ONCE},
      {"Continue", PHP_CONTINUE},
      {"CFunction", PHP_CFUNCTION},
      {"implements", PHP_IMPLEMENTS},
      {""}, {""},
      {"die", PHP_DIE},
      {""}, {""},
      {"define", PHP_DEFINE},
      {"declare", PHP_DECLARE},
      {"Use", PHP_USE},
      {""},
      {"Print", PHP_PRINT},
      {"public", PHP_PUBLIC},
      {""},
      {"Try", PHP_TRY},
      {"True", PHP_TRUE},
      {"array", PHP_ARRAY},
      {""},
      {"do", PHP_DO},
      {""},
      {"_ENV", PHP__ENV},
      {"ENDIF", PHP_ENDIF},
      {"ENDFOR", PHP_ENDFOR},
      {"AS", PHP_AS},
      {"XOR", PHP_XOR},
      {"eval", PHP_EVAL},
      {"Trait", PHP_TRAIT},
      {"Public", PHP_PUBLIC},
      {"FOREACH", PHP_FOREACH},
      {"var", PHP_VAR},
      {""},
      {"Const", PHP_CONST},
      {"_FILES", PHP__FILES},
      {"OR", PHP_OR},
      {"Abstract", PHP_ABSTRACT},
      {"GOTO", PHP_GOTO},
      {"Break", PHP_BREAK},
      {""}, {""}, {""},
      {"ELSE", PHP_ELSE},
      {"FINAL", PHP_FINAL},
      {"return", PHP_RETURN},
      {"FINALLY", PHP_FINALLY},
      {""}, {""},
      {"Class", PHP_CLASS},
      {""},
      {"Include_once", PHP_INCLUDE_ONCE},
      {""}, {""},
      {"Implements", PHP_IMPLEMENTS},
      {""},
      {"require_once", PHP_REQUIRE_ONCE},
      {""},
      {"EVAL", PHP_EVAL},
      {"Array", PHP_ARRAY},
      {""},
      {"Require", PHP_REQUIRE},
      {""},
      {"_GET", PHP__GET},
      {"BREAK", PHP_BREAK},
      {"GLOBAL", PHP_GLOBAL},
      {"GLOBALS", PHP_GLOBALS},
      {"__LINE__", PHP___LINE__},
      {"CASE", PHP_CASE},
      {"enddeclare", PHP_ENDDECLARE},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"_COOKIE", PHP__COOKIE},
      {""},
      {"ECHO", PHP_ECHO},
      {"break", PHP_BREAK},
      {""},
      {"default", PHP_DEFAULT},
      {"TRY", PHP_TRY},
      {"Eval", PHP_EVAL},
      {"empty", PHP_EMPTY},
      {""},
      {"Include_Once", PHP_INCLUDE_ONCE},
      {"Die", PHP_DIE},
      {"LIST", PHP_LIST},
      {"Yield", PHP_YIELD},
      {"Define", PHP_DEFINE},
      {"Declare", PHP_DECLARE},
      {"USE", PHP_USE},
      {""},
      {"FALSE", PHP_FALSE},
      {""},
      {"extends", PHP_EXTENDS},
      {"__FILE__", PHP___FILE__},
      {"exit", PHP_EXIT},
      {"yield", PHP_YIELD},
      {"ELSEIF", PHP_ELSEIF},
      {"Do", PHP_DO},
      {""}, {""},
      {"while", PHP_WHILE},
      {""}, {""},
      {"endwhile", PHP_ENDWHILE},
      {""},
      {"EMPTY", PHP_EMPTY},
      {"Return", PHP_RETURN},
      {"__DIR__", PHP___DIR__},
      {"xor", PHP_XOR},
      {""},
      {"While", PHP_WHILE},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"Require_once", PHP_REQUIRE_ONCE},
      {""},
      {"__CLASS__", PHP___CLASS__},
      {"Enddeclare", PHP_ENDDECLARE},
      {""},
      {"EXTENDS", PHP_EXTENDS},
      {""},
      {"EXIT", PHP_EXIT},
      {"throw", PHP_THROW},
      {""}, {""},
      {"__NAMESPACE__", PHP___NAMESPACE__},
      {""},
      {"CLONE", PHP_CLONE},
      {""},
      {"DO", PHP_DO},
      {"DIE", PHP_DIE},
      {"TRUE", PHP_TRUE},
      {"Empty", PHP_EMPTY},
      {"switch", PHP_SWITCH},
      {"INCLUDE", PHP_INCLUDE},
      {""},
      {"protected", PHP_PROTECTED},
      {"CATCH", PHP_CATCH},
      {""}, {""}, {""},
      {"endswitch", PHP_ENDSWITCH},
      {""}, {""},
      {"Extends", PHP_EXTENDS},
      {""},
      {"Exit", PHP_EXIT},
      {"ARRAY", PHP_ARRAY},
      {""},
      {"Require_Once", PHP_REQUIRE_ONCE},
      {""},
      {"Protected", PHP_PROTECTED},
      {""}, {""},
      {"Default", PHP_DEFAULT},
      {"Endwhile", PHP_ENDWHILE},
      {""}, {""}, {""},
      {"private", PHP_PRIVATE},
      {""}, {""},
      {"UNSET", PHP_UNSET},
      {""}, {""},
      {"EndWhile", PHP_ENDWHILE},
      {"__TRAIT__", PHP___TRAIT__},
      {""}, {""}, {""}, {""}, {""},
      {"ENDFOREACH", PHP_ENDFOREACH},
      {""},
      {"Private", PHP_PRIVATE},
      {""},
      {"INSTEADOF", PHP_INSTEADOF},
      {""}, {""}, {""},
      {"CALLABLE", PHP_CALLABLE},
      {""}, {""},
      {"PUBLIC", PHP_PUBLIC},
      {"PRIVATE", PHP_PRIVATE},
      {""}, {""},
      {"ISSET", PHP_ISSET},
      {"HTTP_COOKIE_VARS", PHP_HTTP_COOKIE_VARS},
      {""}, {""}, {""},
      {"IMPLEMENTS", PHP_IMPLEMENTS},
      {""},
      {"__FUNCTION__", PHP___FUNCTION__},
      {""},
      {"Endswitch", PHP_ENDSWITCH},
      {"EndDeclare", PHP_ENDDECLARE},
      {""},
      {"REQUIRE", PHP_REQUIRE},
      {""}, {""},
      {"_POST", PHP__POST},
      {""},
      {"_SERVER", PHP__SERVER},
      {""}, {""},
      {"PRINT", PHP_PRINT},
      {""}, {""}, {""}, {""},
      {"INSTANCEOF", PHP_INSTANCEOF},
      {"Switch", PHP_SWITCH},
      {""}, {""},
      {"CFUNCTION", PHP_CFUNCTION},
      {"Throw", PHP_THROW},
      {""}, {""},
      {"HTTP_ENV_VARS", PHP_HTTP_ENV_VARS},
      {""}, {""}, {""}, {""},
      {"FUNCTION", PHP_FUNCTION},
      {""}, {""},
      {"DEFINE", PHP_DEFINE},
      {"INCLUDE_ONCE", PHP_INCLUDE_ONCE},
      {"CONTINUE", PHP_CONTINUE},
      {""},
      {"TRAIT", PHP_TRAIT},
      {"STATIC", PHP_STATIC},
      {""},
      {"_REQUEST", PHP__REQUEST},
      {""},
      {"CONST", PHP_CONST},
      {""},
      {"OLD_FUNCTION", PHP_OLD_FUNCTION},
      {""}, {""}, {""}, {""}, {""},
      {"ABSTRACT", PHP_ABSTRACT},
      {""},
      {"WHILE", PHP_WHILE},
      {""},
      {"DECLARE", PHP_DECLARE},
      {"HTTP_GET_VARS", PHP_HTTP_GET_VARS},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
      {"RETURN", PHP_RETURN},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"CLASS", PHP_CLASS},
      {""}, {""},
      {"ENDWHILE", PHP_ENDWHILE},
      {"EndSwitch", PHP_ENDSWITCH},
      {"THROW", PHP_THROW},
      {""}, {""}, {""},
      {"NAMESPACE", PHP_NAMESPACE},
      {"YIELD", PHP_YIELD},
      {""}, {""}, {""}, {""}, {""}, {""},
      {"DEFAULT", PHP_DEFAULT},
      {""},
      {"INTERFACE", PHP_INTERFACE},
      {""}, {""},
      {"REQUIRE_ONCE", PHP_REQUIRE_ONCE},
      {"_SESSION", PHP__SESSION},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
      {"HTTP_SERVER_VARS", PHP_HTTP_SERVER_VARS},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"HTTP_POST_VARS", PHP_HTTP_POST_VARS},
      {"HTTP_POST_FILES", PHP_HTTP_POST_FILES},
      {""}, {""}, {""}, {""},
      {"__METHOD__", PHP___METHOD__},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"HTTP_SESSION_VARS", PHP_HTTP_SESSION_VARS},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
      {"ENDDECLARE", PHP_ENDDECLARE},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"SWITCH", PHP_SWITCH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
      {"ENDSWITCH", PHP_ENDSWITCH},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {"PROTECTED", PHP_PROTECTED}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = php_hash (str, len);

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
php_reserved_word(const char *str, int len)
{
	struct keyword *keyword;

	keyword = php_lookup(str, len);
	return (keyword && IS_RESERVED_WORD(keyword->token)) ? keyword->token : 0;
}
int
php_reserved_variable(const char *str, int len)
{
	struct keyword *keyword;

	keyword = php_lookup(str, len);
	return (keyword && IS_RESERVED_VARIABLE(keyword->token)) ? keyword->token : 0;
}
