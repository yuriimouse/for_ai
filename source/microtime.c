/**
 * @brief Microtime
 * @author Yurii Prudius
 **/
#include "../helper.hpp"
extern "C"
{
#include <stdlib.h>
#include <sys/time.h>

    /**
     * Microseconds since the base or day (if base is 0)
     *
     * @returns unsigned long long
     **/
    unsigned long long microtime(unsigned long long base)
    {
        struct timeval time;
        gettimeofday(&time, NULL);

        unsigned long long microsec = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;

        if (microsec < base)
        {
            microsec += ((unsigned long long)1000000 * 24 * 3600); // next day
        }
        return microsec - base;
    }
}
