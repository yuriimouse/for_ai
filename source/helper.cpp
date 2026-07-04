// helper.cpp
#include "../helper.hpp"

thread_local SMFICTX *sessionCtx = nullptr;
thread_local unsigned long local_sid = 0;

static thread_local char buff[24] = "";
const char *siphash_string(const char *src, const char key[16])
{
    return snprintf(buff, 20, HASH_FORMAT, siphash(src, key)) > 0 ? buff : nullptr;
}
