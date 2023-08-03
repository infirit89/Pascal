#pragma once

#include "Log.h"

// TODO: make debugbreak portable

#if defined(PS_DEBUG)
    #define PS_ASSERT(condition, ...)  \
    do                                 \
    {                                  \
        if(!(condition))               \
        {                              \
            PS_ERROR(__VA_ARGS__);     \
            __builtin_trap();          \
        }                              \
    } while(0)
#endif
