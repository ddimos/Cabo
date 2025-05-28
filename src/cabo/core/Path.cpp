#include "core/Path.hpp"
#include "core/Assert.hpp"

#include <string>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif

namespace cn::core
{

std::filesystem::path getExecutablePath()
{
    char buffer[4096];

#if defined(_WIN32)
    DWORD len = GetModuleFileNameA(NULL, buffer, sizeof(buffer));
    if (len == 0 || len == sizeof(buffer))
    {
        CN_ASSERT_FRM(false, "Failed to get executable path on Windows, {}", len);
        return std::filesystem::path{};
    }
#elif defined(__APPLE__)
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0)
    {
        CN_ASSERT_FRM(false, "Buffer too small for _NSGetExecutablePath on macOS, {}", size);
        return std::filesystem::path{};
    }
#elif defined(__linux__)
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (len == -1 || len == sizeof(buffer))
    {
        CN_ASSERT_FRM(false, "Failed to get executable path on Linux, {}", len);
        return std::filesystem::path{};
    }
#else
    static_assert(false, "Unsupported platform");
#endif

    return std::filesystem::weakly_canonical(std::string(buffer));
}

} // namespace cn::core
