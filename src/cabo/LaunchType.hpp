#pragma once

#if CN_LAUNCH_TYPE != CN_LAUNCH_TYPE_SERVER && CN_LAUNCH_TYPE != CN_LAUNCH_TYPE_CLIENT
    #error Launch type is not properly defined
#endif

namespace cn::launch_type
{

constexpr bool isServer()
{
    return CN_LAUNCH_TYPE == CN_LAUNCH_TYPE_SERVER;
}

constexpr bool isClient()
{
    return CN_LAUNCH_TYPE == CN_LAUNCH_TYPE_CLIENT;
}

} // namespace cn

#define CN_IF_SERVER if constexpr (cn::launch_type::isServer())
#define CN_IF_CLIENT if constexpr (cn::launch_type::isClient())
