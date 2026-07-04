/**
 * Comfortable regular expressions
 * @author Yurii Prudius
 **/
#include "../helper.hpp"
extern "C"
{
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

/**
 * Check if matches the `expr` in the `subj`.
 *
 * @param expr the regular expression
 * @param subj the string
 *
 * @return the integer
 *    0 = matches not found
 *    1 = expression matches
 **/
int regexp_match(char *expr, char *subj)
{
    regex_t regex;
    int reti = 0;

    // Compile regular expression
    if (!(reti = regcomp(&regex, expr, REG_EXTENDED)))
    {
        // Execute regular expression
        reti = regexec(&regex, subj, 0, NULL, 0);
    }
    // Free memory allocated to the pattern buffer by regcomp()
    regfree(&regex);

    return !reti;
}

/**
 * Internal function.
 * Converts regmatch_t records to strings.
 *
 **/
static string *regexp_matchesStatic(regex_t *regex, const char *subj, int cnt)
{
    regmatch_t *pmatches = NULL;
    string *matches = NULL;

    // Allocate matches
    pmatches = (regmatch_t *)calloc(cnt, sizeof(regmatch_t));
    if (pmatches)
    {
        // Execute regular expression
        if (!regexec(regex, subj, cnt, pmatches, 0))
        {
            // cnt+1 for last NULL
            matches = (char **)calloc(cnt + 1, sizeof(string));
            if (matches)
            {
                for (int i = 0; i < cnt; i++)
                {
                    matches[i] = string_get(subj + (pmatches[i].rm_so), (pmatches[i].rm_eo) - (pmatches[i].rm_so));
                }
            }
        }
        // Free pmatches
        free(pmatches);
    }
    return matches;
}

/**
 * Find matches for the `expr` in the `subj`.
 *
 * @param expr the regular expression
 * @param subj the string
 *
 * @return the new allocated array (NULL terminated) of strings | NULL
 * @see regexpFree
 **/
string *regexp_find(const char *expr, const char *subj)
{
    regex_t regex;
    int reti = 0;
    int cnt = string_count(expr, '(') + 1;
    string *matches = NULL;

    // Compile regular expression
    if (!(reti = regcomp(&regex, expr, REG_EXTENDED)))
    {
        matches = regexp_matchesStatic(&regex, subj, cnt);
    }
    // Free memory allocated to the pattern buffer by regcomp()
    regfree(&regex);

    return matches;
}

/**
 * @brief Frees the memory allocated by regexpFind
 **/
char **regexp_free(char **matches)
{
    if (matches)
    {
        for (int i = 0; matches[i]; i++)
        {
            FREE_AND_NULL(matches[i]);
        }
        FREE_AND_NULL(matches);
    }
    return NULL;
}

// Convert a single wildcard pattern into a regex fragment.
// Rules:
// - '*' -> ".*"
// - '~' -> "."
// - '.' -> "\\."
// - Other characters are copied as-is (this lets regex operators like '|' pass through)
string regexp_resolveWildcards(const char *pattern)
{
    size_t len = strlen(pattern);
    // Worst-case: each character might be escaped (2 chars) or expanded (2 chars)
    string buf = (char *)malloc(len * 2 + 1);
    if (buf)
    {
        char *dest = buf;
        for (dest = buf; *pattern; pattern++)
        {
            switch (*pattern)
            {
            case '*':
                *dest++ = '.';
                *dest++ = '*';
                break;
            case '~':
                *dest++ = '.';
                break;
            case '.':
                // escape literal dot
                *dest++ = '\\';
                *dest++ = '.';
                break;
            default:
                *dest++ = *pattern;
                break;
            }
        }
        *dest = '\0';
    }
    return buf;
}

/**
 * Build a regex from a NULL-terminated list of wildcard strings.
 * Each pattern is converted and wrapped in a capturing group.
 *
 * @param first
 * @param ...
 * @return string
 */
string regexp_wildcardsChain(const char *first, ...)
{
    // Start with the beginning anchor.
    string result = (char *)malloc(2); // Enough for "^" and '\0'
    if (result)
    {
        strcpy(result, "^");
        va_list args;
        va_start(args, first);

        for (char *pattern = (char *)first; pattern && result; pattern = va_arg(args, char *))
        {
            char *frag = regexp_resolveWildcards(pattern);
            if (frag)
            {
                // Append group: "(" + fragment + ")"
                result = string_cats(result, "(", frag, ")", NULL);
            }
            else
            {
                FREE_AND_NULL(result);
                break;
            }
        }
        va_end(args);
    }

    if (result)
    {
        // Append the ending anchor.
        result = string_cat(result, "$");
    }
    return result;
}
