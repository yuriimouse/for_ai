/**
 * @brief Email helper
 * @author Yurii Prudius
 **/
#include "../helper.hpp"
extern "C"
{
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
}

/**
 * Modify email
 *
 * Lowercase
 * removes the <> brackets
 * EMAIL_CLEAR_COMMENTS - removes everything after the "+" sign
 * EMAIL_CLEAR_DOTS - removes left dots
 * EMAIL_CLEAR_AT - change @ to dot
 *
 * @param email
 * @param clear
 * @return string | NULL
 */
string email_normalise(const char *email, int clear)
{
    // dup to lowercase
    string buff = string_lower(email);
    // get the part in angle brackets
    string canon = string_splitLast(buff, '<');
    if (canon)
    {
        FREE_AND_NULL(buff);
        string_split(canon, '>'); // remove tail
    }
    else
        canon = buff;

    if (clear)
    {
        // dup domain (`canon` changed)
        string domain = string_splitLast(canon, '@');

        if (clear & EMAIL_CLEAR_COMMENTS)
        { // free '+' tail (`canon` changed)
            string_split(canon, '+');
        }

        string email = domain
                           ? string_cats(canon,
                                         clear & EMAIL_CLEAR_AT ? "." : "@",
                                         domain,
                                         NULL)
                           : string_dup(canon);

        FREE_AND_NULL(domain);
        FREE_AND_NULL(canon);
        return email;
    }
    return canon;
}

string email_auth(const char *email, const char *subst, int clear)
{
    if (string_empty(email) || string_empty(subst))
        return nullptr;

    string base = (!strchr(email, '@') && 1 == string_counter(email, subst))
                      ? string_replace(email, subst, "@@@@@@@@@@")
                      : string_dup(email);
    return EMAIL_CLEAR_AT == clear ? string_inplaceChr(base, '@', '.') : base;
}
