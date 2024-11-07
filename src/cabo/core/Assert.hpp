#pragma once
#include "core/Log.hpp"

#ifdef CN_DISABLE_ASSERT

    #undef CN_ASSERT
    #undef CN_ASSERT_FRM
    #define CN_ASSERT(_cnd) (void(0))
    #define CN_ASSERT_FRM(_cnd, _fmt) (void(0))

#else

    #ifdef CN_ASSERT
        #error  "CN_ASSERT is already defined"
    #endif // CN_ASSERT
    #ifdef CN_ASSERT_FRM
        #error  "CN_ASSERT_FRM is already defined"
    #endif // CN_ASSERT_FRM

    #include <cassert>
    #define CN_ASSERT(_cnd) assert(_cnd)
    #define CN_ASSERT_FRM(_cnd, _fmt, ...)      \
        if (!(_cnd))                            \
        {                                       \
            CN_LOG_ERROR_FRM(_fmt, __VA_ARGS__);\
            assert(_cnd);                       \
        }

#endif // CN_DISABLE_ASSERT
