#pragma once

#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace naida
{
std::shared_ptr<spdlog::logger> get_logger();
} // namespace naida

#define NAIDA_LOG_LEVEL_TRACE 100
#define NAIDA_LOG_LEVEL_INFO  200
#define NAIDA_LOG_LEVEL_WARN  300
#define NAIDA_LOG_LEVEL_ERROR 400

#ifndef NAIDA_LOG_LEVEL
#define NAIDA_LOG_LEVEL NAIDA_LOG_LEVEL_WARN
#endif

// Wrapping the calls in a new fmt::format call since there's an issue with clangd:
//  In template: call to consteval function 'fmt::basic_format_string<char, const char *, const char *&, int
//  &>::basic_format_string<FMT_COMPILE_STRING, 0>' is not a constant expressionclang(invalid_consteval_call)
#if NAIDA_LOG_LEVEL <= NAIDA_LOG_LEVEL_TRACE
#define NAIDA_TRACE(...) ::naida::get_logger()->trace(fmt::format(__VA_ARGS__))
#else
#define NAIDA_TRACE(...)
#endif

#if NAIDA_LOG_LEVEL <= NAIDA_LOG_LEVEL_INFO
#define NAIDA_INFO(...) ::naida::get_logger()->info(fmt::format(__VA_ARGS__))
#else
#define NAIDA_INFO(...)
#endif


#if NAIDA_LOG_LEVEL <= NAIDA_LOG_LEVEL_WARN
#define NAIDA_WARN(...) ::naida::get_logger()->warn(fmt::format(__VA_ARGS__))
#else
#define NAIDA_WARN(...)
#endif
#if NAIDA_LOG_LEVEL <= NAIDA_LOG_LEVEL_ERROR
#define NAIDA_ERROR(...) ::naida::get_logger()->error(fmt::format(__VA_ARGS__))
#else
#define NAIDA_ERROR(...)
#endif
