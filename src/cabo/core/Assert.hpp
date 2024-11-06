#pragma once

#ifdef CN_DISABLE_ASSERT
#    undef CN_ASSERT
#    define CN_ASSERT(condition, message) (void(0))
#elif !defined CN_ASSERT
#    include <cassert>
#    define CN_ASSERT(condition, message) assert(condition && message)
#endif
