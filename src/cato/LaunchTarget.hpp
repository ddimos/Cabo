#pragma once

#if CN_LAUNCH_TARGET == CN_LAUNCH_TARGET_SERVER
    #define CN_SERVER 1
    #define CN_CLIENT 0
#elif CN_LAUNCH_TARGET == CN_LAUNCH_TARGET_CLIENT
    #define CN_SERVER 0
    #define CN_CLIENT 1
#else
    #error Launch target is not properly defined
#endif
