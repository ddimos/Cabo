#pragma once

namespace cn::core::log
{
    
enum class Level
{
    Debug,
    Info,
    Error
};

constexpr Level g_logLevel = Level::Info;

} // namespace cn::core::log

#ifdef CN_DISABLE_LOGGING

    #undef CN_LOG
    #undef CN_LOG_D
    #undef CN_LOG_I
    #undef CN_LOG_E
    #undef CN_LOG_FRM
    #undef CN_LOG_D_FRM
    #undef CN_LOG_I_FRM
    #undef CN_LOG_E_FRM

    #define CN_LOG(_level, _msg) (void(0))
    #define CN_LOG_D(_msg) (void(0))
    #define CN_LOG_I(_msg) (void(0))
    #define CN_LOG_E(_msg) (void(0))
    #define CN_LOG_FRM(_level, _fmt, ...) (void(0))
    #define CN_LOG_D_FRM(_fmt, ...) (void(0))
    #define CN_LOG_I_FRM(_fmt, ...) (void(0))
    #define CN_LOG_E_FRM(_fmt, ...) (void(0))

#else

    #ifdef CN_LOG
        #error  "CN_LOG is already defined"
    #endif // CN_LOG
    #ifdef CN_LOG_D
        #error  "CN_LOG_D is already defined"
    #endif // CN_LOG_D
    #ifdef CN_LOG_I
        #error  "CN_LOG_I is already defined"
    #endif // CN_LOG_I
    #ifdef CN_LOG_E
        #error  "CN_LOG_E is already defined"
    #endif // CN_LOG_E
    #ifdef CN_LOG_FRM
        #error  "CN_LOG_FRM is already defined"
    #endif // CN_LOG_FRM
    #ifdef CN_LOG_D_FRM
        #error  "CN_LOG_D_FRM is already defined"
    #endif // CN_LOG_D_FRM
    #ifdef CN_LOG_I_FRM
        #error  "CN_LOG_I_FRM is already defined"
    #endif // CN_LOG_I_FRM
    #ifdef CN_LOG_E_FRM
        #error  "CN_LOG_E_FRM is already defined"
    #endif // CN_LOG_E_FRM

    #include <format>
    #include <iostream>
    #include <string_view>
    #define CN_LOG(_level, _msg) cn::core::log::log(_level, _msg)
    #define CN_LOG_D(_msg) cn::core::log::log(cn::core::log::Level::Debug, _msg)
    #define CN_LOG_I(_msg) cn::core::log::log(cn::core::log::Level::Info, _msg)
    #define CN_LOG_E(_msg) cn::core::log::log(cn::core::log::Level::Error, _msg)
    #define CN_LOG_FRM(_level, _fmt, ...) cn::core::log::logFmt(_level, _fmt, __VA_ARGS__)
    #define CN_LOG_D_FRM(_fmt, ...) cn::core::log::logFmt(cn::core::log::Level::Debug, _fmt, __VA_ARGS__)
    #define CN_LOG_I_FRM(_fmt, ...) cn::core::log::logFmt(cn::core::log::Level::Info, _fmt, __VA_ARGS__)
    #define CN_LOG_E_FRM(_fmt, ...) cn::core::log::logFmt(cn::core::log::Level::Error, _fmt, __VA_ARGS__)

namespace cn::core::log
{

inline void flush()
{
    std::cout << std::flush;
}

inline void log(Level _level, std::string_view _msg)
{
    if (g_logLevel > _level)
        return;
    std::cout << _msg << '\n';
    if (_level == Level::Error)
        cn::core::log::flush();
}

template<typename... Args>
inline void logFmt(Level _level, const std::format_string<Args...> _fmt, Args&&... _args)
{
    if (g_logLevel > _level)
        return;
    std::cout << std::vformat(_fmt.get(), std::make_format_args(_args...)) << '\n';
    if (_level == Level::Error)
        cn::core::log::flush();
}

} // namespace cn::core::log

#endif // CN_DISABLE_LOGGING
