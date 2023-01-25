#pragma once
#ifndef SLSFS_DEBUGLOG_HPP__
#define SLSFS_DEBUGLOG_HPP__

#include "http-verb.hpp"
#include "fmt/core.h"

#include <curl/curl.h>

#include <thread>
#include <memory>
#include <chrono>


namespace slsfs::log
{

enum class level
{
    trace = 0,
    debug,
    info,
    warning,
    error,
    fatal,
    none
};

namespace detail
{

struct global_info
{
    char const ** signature;
    std::chrono::high_resolution_clock::time_point start;
    static constexpr bool to_remote = false;

#ifdef NDEBUG
    static constexpr level current_level = level::info;
#else
    static constexpr level current_level = level::trace;
#endif // NDEBUG

};

auto global_info_instance() -> global_info&
{
    static global_info info;
    return info;
}

template<level Level = level::trace>
void logstring(std::string const & msg)
{
    auto const now = std::chrono::high_resolution_clock::now();
    global_info& info = global_info_instance();
    auto relativetime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - info.start).count();

    if constexpr (global_info::current_level <= Level)
    {
        std::string const finalmsg = fmt::format("[{0:12d} {1}] {2}", relativetime, (*info.signature), msg);
        std::cerr << finalmsg << std::endl;
    }
    return;
}

} // namespace

auto init(char const * &signature)
{
    detail::global_info& info = detail::global_info_instance();
    info.start = std::chrono::high_resolution_clock::now();
    info.signature = std::addressof(signature);

    detail::logstring(fmt::format("{} unixtime",
                                  std::chrono::duration_cast<std::chrono::nanoseconds>(
                                      info.start.time_since_epoch()).count()));
    return info;
}

template<level Level = level::trace, typename ... Args>
void log(fmt::format_string<Args...> fmt, Args&& ... args)
{
    if constexpr (detail::global_info::current_level <= Level)
        detail::logstring(fmt::format(fmt, std::forward<Args>(args)...));
    return;
}

template<level Level = level::trace>
void log(std::string const& str)
{
    if constexpr (detail::global_info::current_level <= Level)
        detail::logstring(str);
    return;
}

template<level Level = level::trace>
void log(char const* str)
{
    if constexpr (detail::global_info::current_level <= Level)
        detail::logstring(str);
    return;
}

} // namespace slsfs::log

#endif // SLSFS_DEBUGLOG_HPP__
