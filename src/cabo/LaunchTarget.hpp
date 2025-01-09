#pragma once

#if CN_LAUNCH_TARGET != CN_LAUNCH_TARGET_SERVER && CN_LAUNCH_TARGET != CN_LAUNCH_TARGET_CLIENT
    #error Launch target is not properly defined
#endif

namespace cn::launch_target
{

constexpr bool isServer()
{
    return CN_LAUNCH_TARGET == CN_LAUNCH_TARGET_SERVER;
}

constexpr bool isClient()
{
    return CN_LAUNCH_TARGET == CN_LAUNCH_TARGET_CLIENT;
}

} // namespace cn::launch_target

#define CN_IF_SERVER constexpr (cn::launch_target::isServer())
#define CN_IF_CLIENT constexpr (cn::launch_target::isClient())
