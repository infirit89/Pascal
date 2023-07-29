#pragma once

#include "Log.h"

#if defined(PS_DEBUG)
    #define PS_ASSERT(condition, ...)  \
    do                                 \
    {                                  \
        if(!(condition))               \
        {                              \
            PS_ERROR(__VA_ARGS__);     \
            __debugbreak();            \
        }                              \
    } while(0)
#endif
