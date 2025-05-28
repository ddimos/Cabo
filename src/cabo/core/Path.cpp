#include "core/Path.hpp"

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
        throw std::runtime_error("Failed to get executable path on Windows");
#elif defined(__APPLE__)
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) != 0)
        throw std::runtime_error("Buffer too small for _NSGetExecutablePath on macOS");
#elif defined(__linux__)
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (len == -1 || len == sizeof(buffer))
        throw std::runtime_error("Failed to get executable path on Linux");
#else
    throw std::runtime_error("Unsupported platform");
#endif

    return std::filesystem::weakly_canonical(std::string(buffer));
}

} // namespace cn::core
