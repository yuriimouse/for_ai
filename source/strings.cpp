/**
 * @brief Comfortable strings
 * @author Yurii Prudius
 **/
#include "../helper.hpp"
extern "C"
{
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

/**
 * Count the char
 *
 * @return int
 **/
int string_count(const char *str, char chr)
{
    int cnt = 0;
    if (!string_empty(str))
    {
        for (char *ptr = (char *)str; *ptr; ptr++)
        {
            if (chr == *ptr)
            {
                cnt++;
            }
        }
    }
    return cnt;
}

/**
 * Count the list of chars
 *
 * @return int
 **/
int string_counter(const char *str, char *chrs)
{
    int cnt = 0;
    if (!string_empty(str) && !string_empty(chrs))
    {
        for (char *ptr = (char *)str; *ptr; ptr++)
        {
            if (strchr(chrs, *ptr))
            {
                cnt++;
            }
        }
    }
    return cnt;
}

/**
 * Checks whether all characters are lowercase
 *
 * @param src
 * @return bool
 */
bool string_isSnake(const char *src)
{
    for (char *ptr = (char *)src; *ptr; ptr++)
    {
        if (isupper(*ptr))
        {
            return 0;
        }
    }
    return 1;
}

/**
 * Checks if only the first character is uppercase and the rest are lowercase
 *
 * @param src
 * @return bool
 */
bool string_isCamel(const char *src)
{
    if (!isupper(*src))
    {
        return 0;
    }
    return string_isSnake(src + 1);
}

/**
 * Reverse comparison
 *
 * @param str
 * @param suffix
 * @return int
 */
bool string_endsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return false;
    size_t ls = strlen(str);
    size_t lt = strlen(suffix);
    if (lt > ls)
        return false;

    return 0 == memcmp(str + ls - lt, suffix, lt);
}

char *string_default(const char *str, const char *def)
{
    return string_empty(str) ? (char *)def : (char *)str;
}

/**
 * Duplicates a non-empty string.
 *
 * @returns string | NULL
 **/
string string_dup(const char *src)
{
    if (!string_empty(src))
    {
        return strdup(src);
    }
    return NULL;
}

/**
 * Duplicates a non-empty part of the non-empty string.
 *
 * @returns string | NULL
 **/
string string_get(const char *src, size_t count)
{
    if (src && count)
    {
        if (count < strlen(src))
        {
            string mem = (char *)malloc(count + 1);
            if (mem)
            {
                strncpy(mem, src, count);
            }
            return mem;
        }
        else
        {
            return strdup(src);
        }
    }
    return NULL;
}

/**
 * Duplicates a non-empty string to upper case.
 *
 * @returns string | NULL
 **/
string string_upper(const char *src)
{
    if (!string_empty(src))
    {
        string r = strdup(src);
        for (string p = r; *p; p++)
        {
            *p = toupper(*p);
        }
        return r;
    }
    return NULL;
}

/**
 * Duplicates a non-empty string to lower case.
 *
 * @returns string | NULL
 **/
string string_lower(const char *src)
{
    if (!string_empty(src))
    {
        string r = strdup(src);
        for (string p = r; *p; p++)
        {
            *p = tolower(*p);
        }
        return r;
    }
    return NULL;
}

/**
 * Frees the allocated string.
 *
 * @returns NULL
 **/
string string_free(string src)
{
    if (src)
    {
        free((void *)src);
    }
    return NULL;
}

/**
 * Checks if a word is in the string
 *
 * @param str
 * @param word
 * @param delim
 * @return bool
 */
bool string_found(const char *str, const char *word, const char *delim)
{
    int ret = 0;
    if (str && word && delim)
    {
        string dup = strdup(str);
        string_forEach(dup, delim, token)
        {
            ret = string_equals(word, token);
            if (ret)
            {
                break;
            }
        }
        FREE_AND_NULL(dup);
    }
    return ret;
}

/**
 * Concatenates two strings.
 * Equivalent for stringCats(src,piece,NULL).
 *
 * @returns string | NULL
 **/
string string_cat(const char *src, const char *piece)
{
    if (!string_empty(src))
    {
        if (!string_empty(piece))
        {
            string snew = (char *)malloc(strlen(src) + strlen(piece) + 1);
            strcpy(snew, src);
            strcat(snew, piece);
            return snew;
        }
        return strdup(src);
    }
    return string_dup(piece);
}

/**
 * Appends the repeated non-empty piece to the non-empty string.
 *
 * @returns string | NULL
 **/
string string_catRepeat(const char *src, const char *piece, size_t count)
{
    string snew = NULL;

    if (string_empty(piece) || !count)
    {
        return string_dup(src);
    }
    if (string_empty(src))
    {
        snew = (char *)malloc(count * strlen(piece) + 1);
    }
    else
    {
        snew = (char *)malloc(strlen(src) + count * strlen(piece) + 1);
        strcpy(snew, src);
    }
    while (count--)
    {
        strcat(snew, piece);
    }

    return snew;
}

/**
 * Concatenates a NULL-terminated sequence of strings.
 *
 * @returns string | NULL
 **/
string string_cats(const char *src, const char *piece, ...)
{
    va_list valist;
    va_start(valist, piece);

    string ptr = string_cat(src, piece);
    if (!string_empty(ptr))
    {
        char *arg;
        while ((arg = va_arg(valist, string)))
        {
            ptr = (string)realloc(ptr, strlen(ptr) + strlen(arg) + 1);
            strcat(ptr, arg);
        }
    }

    va_end(valist);
    return ptr;
}

string string_expand(char *src, const char *piece)
{
    if (src)
    {
        if (piece)
        {
            char *ex = (char *)realloc(src, strlen(src) + strlen(piece) + 1);
            return ex ? strcat(ex, piece) : src;
        }
        return src;
    }
    return string_dup(piece);
}

string string_join(const char *delim, const char *piece, ...)
{
    va_list valist;
    va_start(valist, piece);

    char *ptr = string_dup(piece);
    char *arg;
    while ((arg = va_arg(valist, char *)))
    {
        ptr = (char *)realloc(ptr, strlen(ptr) + strlen(arg) + strlen(delim) + 1);
        strcat(ptr, delim);
        strcat(ptr, arg);
    }

    va_end(valist);
    return ptr;
}

/**
 * Replaces multiple ws into single
 *
 * @param src
 * @return string
 */
string string_unfold(const char *src)
{
    string out = (char *)malloc(strlen(src) + 1);
    char *iwrite = out;

    bool is_spaced = false;
    for (char *iread = (char *)src; *iread; iread++)
    {
        if (isspace(*iread))
        {
            is_spaced = true;
            continue;
        }
        else if (is_spaced)
            *iwrite++ = ' ';

        is_spaced = false;
        *iwrite++ = *iread;
    }
    *iwrite = '\0';

    return out;
}

/**
 * Replaces a char to the another char.
 *
 * @returns string | NULL
 **/
string string_replaceChr(const char *str, char find, char replace)
{
    string snew = string_dup(str);
    if (!string_empty(snew))
    {
        string ix = snew;

        while (ix && ((ix = strchr(ix, find)) != NULL))
        {
            *ix++ = replace;
        }
    }
    return snew;
}

/**
 * Inplace replaces a char to the another char.
 *
 * @returns string changed inplace
 **/
char *string_inplaceChr(char *str, char find, char replace)
{
    if (!string_empty(str))
    {
        for (char *ix = (char *)str; *ix; ix++)
        {
            if (*ix == find)
                *ix = replace;
        }
    }
    return str;
}

/**
 * @brief Replaces chars to another chars.
 *
 * @returns string | NULL
 **/
string string_replace(const char *str, const char *find, const char *replace)
{
    if (!string_empty(str) && !string_empty(find) && !string_empty(replace))
    {

        size_t len = std::min(strlen(replace), strlen(find));
        string sstr = strdup(str);
        string ix;

        for (size_t iii = 0; iii < len; iii++)
        {
            ix = sstr;
            while ((ix = strchr(ix, find[iii])) != NULL)
            {
                *ix++ = replace[iii];
            }
        }
        return sstr;
    }

    return NULL;
}

/**
 * Replaces all occurrences of the search string with the replacement string in the given input string.
 *
 * @param str      The input string in which replacements should be performed.
 * @param find     The substring to search for within the input string. If NULL, returns a copy of `str`.
 * @param replace  The substring to replace each occurrence of `find` with. If NULL, occurrences of `find` are removed.
 *
 * @returns A new dynamically allocated string with replacements applied, or NULL if memory allocation fails.
 *          The caller is responsible for freeing the returned string.
 */
string string_modify(const char *str, const char *find, const char *replace)
{
    if (!string_empty(str))
    {
        if (string_empty(find))
        {
            return strdup(str);
        }
        if (!replace)
        {
            replace = ""; // If replace is NULL, treat it as an empty string (removal)
        }

        size_t str_len = strlen(str);
        size_t find_len = strlen(find);
        size_t replace_len = strlen(replace);

        // Count occurrences of `find` in `str`
        size_t count = 0;
        const char *temp = str;
        while ((temp = strstr(temp, find)))
        {
            count++;
            temp += find_len;
        }

        // If no occurrences, return a copy of `str`
        if (count == 0)
        {
            return strdup(str);
        }

        // Allocate memory for the new string
        size_t new_len = str_len + (replace_len - find_len) * count;
        char *new_str = (char *)malloc(new_len + 1);
        if (!new_str)
        {
            return NULL; // Allocation failure
        }

        // Perform replacement
        char *dst = new_str;
        const char *src = str;
        while (*src)
        {
            if (strncmp(src, find, find_len) == 0)
            {
                // Copy replacement string
                strcpy(dst, replace);
                dst += replace_len;
                src += find_len;
            }
            else
            {
                // Copy character from original string
                *dst++ = *src++;
            }
        }
        *dst = '\0'; // Null-terminate the new string
        return new_str;
    }
    return NULL;
}

/**
 * Removes all occurrences of characters in `removes` from `str`.
 *
 * @param str     The input string to modify in-place.
 * @param removes A null-terminated string containing characters to remove.
 *
 * @returns The modified input string.
 */
char *string_remove(char *str, const char *removes)
{
    if (string_empty(str) || string_empty(removes))
    {
        return str; // Handle NULL inputs gracefully
    }

    char *src = str, *dst = str; // Two-pointer approach
    while (*src)
    {
        if (!strchr(removes, *src))
        {
            *dst++ = *src; // Copy character if not in `removes`
        }
        src++;
    }
    *dst = '\0'; // Null-terminate the new string

    return str;
}

/**
 * Replaces or removes specified characters in the string in place.
 *
 * @param str     The input string to modify in place.
 * @param find    Characters to find and replace/remove.
 * @param replace Characters to replace corresponding `find` characters with.
 *                If `replace` is shorter than `find`, extra `find` characters are removed.
 *
 * @returns The modified input string.
 */
char *string_change(char *str, const char *find, const char *replace)
{
    if (string_empty(str) || string_empty(find))
        return str; // Handle NULL pointers

    int replace_len = strlen(replace);
    char *src = str, *dst = str;
    while (*src)
    {
        const char *pos = strchr(find, *src);
        if (pos)
        {
            int index = pos - find;
            if (index < replace_len)
            {
                *dst++ = replace[index]; // Replace character
            }
            // Else, character is removed (skip copying it)
        }
        else
        {
            *dst++ = *src; // Keep character
        }
        src++;
    }

    *dst = '\0'; // Null-terminate the modified string
    return str;
}

/**
 * vasprintf to the new string.
 *
 * @returns string | NULL
 **/
string string_vprintf(const char *format, va_list valist)
{
    char *buff = NULL;
    if (!string_empty(format))
    {
        vasprintf(&buff, format, valist);
    }
    return buff;
}

static thread_local char buff[1024] = "";
const char *string_make(const char *format, ...)
{
    va_list valist;
    va_start(valist, format);
    int cnt = vsnprintf(buff, 1023, format, valist);
    va_end(valist);
    return cnt > 0 ? buff : nullptr;
}

/**
 * aprintf to the new string.
 *
 * @returns string | NULL
 **/
string string_printf(const char *format, ...)
{
    va_list valist;
    va_start(valist, format);
    string st = string_vprintf(format, valist);
    va_end(valist);
    return st;
}

/**
 * Splits a string in place at the first occurrence of `split`.
 *
 * - The function modifies `str` by replacing the `split` character with `'\0'`.
 * - Returns a pointer to the part **after** the split character.
 * - If `split` is not found, returns NULL.
 * - Do **not** free the returned pointer — free only the original string.
 *
 * @param str   The input string to modify.
 * @param split The delimiter character to split at.
 * @returns Pointer to the substring after `split`, or NULL if `split` is not found.
 */
char *string_split(char *str, char split)
{
    if (!string_empty(str) && split)
    {
        char *ptr = strchr(str, split);
        if (ptr)
        {
            *ptr = '\0';    // Null-terminate first part
            return ptr + 1; // Return pointer to next part
        }
    }
    return NULL;
}

/**
 * Splits a string in place at the last occurrence of `split`.
 *
 * - The function modifies `str` by replacing the `split` character with `'\0'`.
 * - Returns a pointer to the part **after** the split character.
 * - If `split` is not found, returns NULL.
 * - Do **not** free the returned pointer — free only the original string.
 *
 * @param str   The input string to modify.
 * @param split The delimiter character to split at.
 * @returns Pointer to the substring after `split`, or NULL if `split` is not found.
 */
char *string_rsplit(char *str, char split)
{
    if (!string_empty(str) && split)
    {
        char *ptr = strrchr(str, split);
        if (ptr)
        {
            *ptr = '\0';    // Null-terminate first part
            return ptr + 1; // Return pointer to next part
        }
    }
    return NULL;
}

/**
 * Splits the string by first occurence of the char.
 *
 * Warning! This function change the `str` argument!
 * The '\0' char will be placed in the first occurence of the `split` char in the 'str' argument.
 *
 * Returns the pointer to the new string (copy of the rest string).
 * @returns string | NULL
 **/
string string_splitFirst(char *str, char split)
{
    return string_dup(string_split(str, split));
}

/**
 * Splits the string by last occurence of the char.
 *
 * Warning! This function change the `str` argument!
 * The '\0' char will be placed in the last occurence of the `split` char in the 'str' argument.
 *
 * Returns the pointer to the new string (copy of the rest string).
 * @returns string | NULL
 **/
string string_splitLast(char *str, char split)
{
    if (!string_empty(str) && split)
    {
        char *ptr = strrchr(str, split);
        if (ptr)
        {
            *ptr = '\0';
            return string_dup(ptr + 1);
        }
    }
    return NULL;
}

/**
 * Inplace removes surrounding quotes from a string (if present).
 *
 * Warning! This function change the `str` argument!
 * Don't free this pointer! Just free the original string!
 *
 * @param str The input string (modified in-place).
 * @return Pointer to the unquoted string (inside the original buffer).
 */
char *string_unquote(char *str, char sym)
{
    if (string_empty(str))
        return str;
    str = string_skip(str);

    char quote[2] = " ";
    quote[0] = sym ? sym : str[0];
    return string_trim(str, quote);
}

/**
 * Inplace trim leading and ending chars in the string.
 *
 * Warning! This function change the `str` argument!
 * Don't free this pointer! Just free the original string!
 *
 * @returns char * | NULL
 **/
char *string_trim(char *str, const char *trim)
{
    if (string_empty(str))
        return str;
    if (string_empty(trim))
        trim = " ";
    str = string_skip(str, trim);

    if (!*str)
        return str;
    for (char *end = str + strlen(str) - 1; (end > str) && strchr(trim, *end); end--)
        *end = '\0';
    return str;
}

/**
 * Skip leading characters
 *
 * @param str
 * @param skip
 * @return char*
 */
char *string_skip(const char *str, const char *skip)
{
    if (string_empty(str))
        return (char *)str;
    if (string_empty(skip))
        skip = " ";

    while (*str && strchr(skip, *str))
        str++;
    return (char *)str;
}
