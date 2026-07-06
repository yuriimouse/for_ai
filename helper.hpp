// helper.hpp
#pragma once
// #ifndef _GNU_SOURCE
// #define _GNU_SOURCE
// #endif
//
typedef char *string;

// #include "json/json.hpp"

// #include <atomic>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
// #include <string>
extern "C"
{
#include <inttypes.h>
#include <libmilter/mfapi.h>
#include <sys/syscall.h>
}
// globals
extern char selfPath[];   // skeleton.cpp
extern char configPath[]; // skeleton.cpp
extern char selfName[];   // skeleton.cpp
extern char sType[];      // skeleton.cpp
extern char hashKey[];    // skeleton.cpp
extern char workDir[];    // skeleton.cpp
extern char pidName[];    // skeleton.cpp
extern char socketName[]; // skeleton.cpp

// logging
extern int logSystem;   // skeleton.cpp
extern char logLocal[]; // skeleton.cpp
extern int logDebug;    // skeleton.cpp

// controls
extern bool ruleErrors;   // skeleton.cpp
extern bool entityErrors; // skeleton.cpp
extern bool assignsCheck; // skeleton.cpp
extern char emptyVar[];   // skeleton.cpp

// thread_local
extern thread_local SMFICTX *sessionCtx;                   // helper.cpp
extern thread_local unsigned long local_sid;               // helper.cpp
extern thread_local unsigned long unreliable_info_counter; // dataspace/dataspace.cpp

#define NUL '\0'
#define FREE_AND_NULL(x) \
    if (x)               \
        free(x);         \
    x = NULL

#define thread_id() (long)syscall(SYS_gettid)

#define HASH_FORMAT "%016" PRIX64
uint64_t siphash24(const void *src, unsigned long src_sz, const char key[16]);
#define siphashn(src, len, key) siphash24((void *)(src), (unsigned long)(len), key)
#define siphash(src, key) siphash24((void *)(src), (unsigned long)strlen(src), key)
const char *siphash_string(const char *src, const char key[16]);

extern "C"
{
    unsigned long long microtime(unsigned long long base);
}

#define EMAIL_CLEAR_COMMENTS 1
#define EMAIL_CLEAR_DOTS 2
#define EMAIL_CLEAR_AT 4

string email_normalise(const char *email, int clear);
#define email_canonise(email) email_normalise(email, EMAIL_CLEAR_COMMENTS)
string email_auth(const char *email, const char *subst, int clear = 0);

int regexp_match(const char *expr, const char *subj);
#define regexp_found(expr, subj) regexp_match((expr), (subj))
#define regexp_matches(expr, subj) regexp_find((expr), (subj))
string *regexp_find(const char *expr, const char *subj);
string *regexp_free(const char **matches);
string regexp_resolveWildcards(const char *pattern);
string regexp_wildcardsChain(const char *first, ...);

/* Check if contains substr */
#define string_contains(haystack, needle) ((haystack) && (needle) && !!strcasestr(haystack, needle))
bool string_found(const char *str, const char *word, const char *delim);
/* Reverse comparison */
bool string_endsWith(const char *str, const char *tail);
/* Checks whether all characters are lowercase */
bool string_isSnake(const char *src);
/* Checks if only the first character is uppercase and the rest are lowercase */
bool string_isCamel(const char *src);
/* Check if equals */
#define string_equals(src, cmp) ((src) && (cmp) && !strcasecmp(src, cmp))
/* Check if starts from */
#define string_startsFrom(src, cmp) ((src) && (cmp) && !strncasecmp(src, cmp, strlen(cmp)))
/* Count the char */
int string_count(const char *str, char chr);
/* Count the chars from list */
int string_counter(const char *str, const char *chrs);
/* Returns NULL */
string string_free(string src);
/* Check if empty */
#define string_empty(str) (!(str && *str))
/* Substitute default if empty */
char *string_default(const char *str, const char *def);

/* !!! All returned strings are in the heap and must be freed after using !!!! */
string string_dup(const char *src);
string string_get(const char *src, size_t count);
inline string string_set(const char *src, size_t count = 0) { return count ? string_get(src, count) : string_dup(src); };
string string_upper(const char *src);
string string_lower(const char *src);

string string_replaceChr(const char *str, char find, char replace);
string string_replace(const char *str, const char *find, const char *replace);
string string_modify(const char *str, const char *find, const char *replace);
char *string_inplaceChr(char *str, char find, char replace);

string string_printf(const char *format, ...);
string string_vprintf(const char *format, va_list valist);

string string_cat(const char *src, const char *piece);
string string_catRepeat(const char *src, const char *piece, size_t count);
string string_cats(const char *src, const char *piece, ...);
string string_expand(char *src, const char *piece);
string string_unfold(const char *src);
string string_join(const char *delim, const char *piece, ...);
inline string string_join(const char *delim, char **argv)
{
    if (!argv[0])
        return nullptr;
    string result = string_dup(argv[0]);

    while (*++argv)
    {
        char *n_res = (char *)realloc(result, strlen(result) + strlen(*argv) + strlen(delim) + 1);
        if (!n_res)
        {
            FREE_AND_NULL(result);
            break;
        }
        result = n_res;
        strcat(result, delim);
        strcat(result, *argv);
    }
    return result;
}

/* !!! Argument `str` is changes !!! */
string string_splitFirst(char *str, char split);
string string_splitLast(char *str, char split);
#define string_splitPath(path) string_splitLast(path, '/')

//
char *string_remove(char *str, const char *removes);
char *string_change(char *str, const char *find, const char *replace);
char *string_rsplit(char *str, char split);
char *string_split(char *str, char split);

char *string_skip(const char *str, const char *skip = nullptr);
static inline char *string_scan(char **ptr, const char *delim) { return (ptr != NULL && *ptr != NULL) ? strtok_r(NULL, delim, ptr) : NULL; }

char *string_trim(char *str, const char *trim = nullptr);
char *string_unquote(char *str, char sym = NUL);

#define _GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME

#define string_forEach3(str, delim, token)                              \
    if (str)                                                            \
        for (char *_sptr = NULL, *token = strtok_r(str, delim, &_sptr); \
             !string_empty(token);                                      \
             token = strtok_r(NULL, delim, &_sptr))

#define string_forEach4(str, delim, token, saveptr)                         \
    if (str)                                                                \
        for (char *saveptr = NULL, *token = strtok_r(str, delim, &saveptr); \
             !string_empty(token);                                          \
             token = strtok_r(NULL, delim, &saveptr))

#define string_forEach(...) _GET_MACRO(__VA_ARGS__, string_forEach4, string_forEach3)(__VA_ARGS__)
