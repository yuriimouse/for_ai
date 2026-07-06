
#include "logger.hpp"
#include <cstdio>

extern "C"
{
#include <stdarg.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
}

/**
 * Info logging
 *
 * @param json
 **/
void log_post(cJSON *json)
{
    if (logSystem && json)
    {
        char *jstr = cJSON_PrintUnformatted(json);
        if (jstr)
        {
            openlog(selfName, LOG_OPTIONS, LOG_FACILITY);
            syslog(LOG_INFO, "%s", jstr);
        }
        FREE_AND_NULL(jstr);
    }
}

/**
 * Local debug logging
 *
 * @param key
 * @param value
 * ...
 **/
static void log_local(const char *key, const char *value)
{
    char *fpath = NULL;
    asprintf(&fpath, logLocal, time(NULL) / (60 * 60 * 24));
    if (fpath)
    {
        FILE *fp = fopen(fpath, "at");
        if (fp)
        {
            fprintf(fp, "[S%lu:T%ld] {\"%s\":%s}\n", local_sid, thread_id(), key, value);
            fclose(fp);
        }
    }
    FREE_AND_NULL(fpath);
}

/**
 * Debug logging
 *
 * @param key
 * @param json
 * ...
 **/
void log_debugJson(const char *key, cJSON *json, va_list args)
{
    if (*logLocal)
    {
        char *jstr = cJSON_PrintUnformatted(json);
        if (jstr)
        {
            char *skey = NULL;
            vasprintf(&skey, key, args);

            log_local(skey, jstr);
            FREE_AND_NULL(skey);
        }
        FREE_AND_NULL(jstr);
    }
}

/**
 * Debug logging
 *
 * @param key
 * @param format
 * ...
 **/
void log_debugString(const char *key, const char *format, va_list args)
{
    if (*logLocal)
    {
        char *skey = NULL;
        vasprintf(&skey, key, args);
        char *str = NULL;
        vasprintf(&str, format, args);

        if (skey && str)
        {
            log_local(skey, str);
        }
        FREE_AND_NULL(str);
        FREE_AND_NULL(skey);
    }
}
