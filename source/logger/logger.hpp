// logger/logger.h
#pragma once
#include "../helper.hpp"
#include "../json/json.hpp"
#include <atomic>
#include <execinfo.h>

#define LOG_OPTIONS LOG_PID
#define LOG_FACILITY LOG_MAIL

static inline int _log_level()
{
    void *stack[64];
    int n = backtrace(stack, 64);
    return n - 3;
};

void log_post(cJSON *json);

void log_debugJson(const char *key, cJSON *json, va_list args);
void log_debugString(const char *key, const char *fmt, va_list args);

inline void log_debug(const char *key, cJSON *json, ...)
{
    if (logLocal[0] && (logDebug > _log_level()))
    {
        va_list args;
        va_start(args, json);
        log_debugJson(key, json, args);
        va_end(args);
    }
};

inline void log_debug(const char *key, const char *fmt, ...)
{
    if (logLocal[0] && (logDebug > _log_level()))
    {
        va_list args;
        va_start(args, fmt);
        log_debugString(key, fmt, args);
        va_end(args);
    }
};
