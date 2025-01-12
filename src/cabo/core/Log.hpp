#pragma once

#ifdef CN_DISABLE_LOGGING

    #undef CN_LOG
    #undef CN_LOG_FRM
    #undef CN_LOG_ERROR_FRM
    #define CN_LOG(_fmt, ...) (void(0))
    #define CN_LOG_FRM(_fmt, ...) (void(0))
    #define CN_LOG_ERROR_FRM(_fmt, ...) (void(0))

#else

    #ifdef CN_LOG
        #error  "CN_LOG is already defined"
    #endif // CN_LOG
    #ifdef CN_LOG_FRM
        #error  "CN_LOG_FRM is already defined"
    #endif // CN_LOG_FRM
    #ifdef CN_LOG_ERROR_FRM
        #error  "CN_LOG_ERROR_FRM is already defined"
    #endif // CN_LOG_ERROR_FRM

    #include <format>
    #include <iostream>
    #include <string_view>
    #define CN_LOG(_msg) cn::core::log::log(_msg)
    #define CN_LOG_FRM(_fmt, ...) cn::core::log::logFmt(_fmt, __VA_ARGS__)
    #define CN_LOG_ERROR_FRM(_fmt, ...)             \
        do {                                        \
            cn::core::log::logFmt(_fmt, __VA_ARGS__);   \
            cn::core::log::flush();                     \
        } while(0)

namespace cn::core::log
{

inline void log(std::string_view _msg)
{
    std::cout << _msg << '\n';
}

template<typename... Args>
inline void logFmt(const std::format_string<Args...> _fmt, Args&&... _args)
{
    std::cout << std::vformat(_fmt.get(), std::make_format_args(_args...)) << '\n';
}

inline void flush()
{
    std::cout << std::flush;
}

} // namespace cn::core::log

#endif // CN_DISABLE_LOGGING
