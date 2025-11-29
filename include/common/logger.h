#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>
#include <filesystem>
#include <string>

namespace InstantSocial
{
    extern std::shared_ptr<spdlog::logger> g_logger;

    inline const char* get_filename(const char *full_path) {
        const char *filename = std::strrchr(full_path, '/');
        return filename ? filename + 1 : full_path;
    }

    // 颜色控制代码
#ifdef _WIN32
    // Windows 颜色代码
    class WindowsColor {
    public:
        static void setColor(int level);
        static void resetColor();
    };
#else
    // ANSI 颜色代码
    namespace Color {
        static const std::string Reset = "\033[0m";
        static const std::string Bright = "\033[1m";
        
        // 前景色
        static const std::string Black = "\033[30m";
        static const std::string Red = "\033[31m";
        static const std::string Green = "\033[32m";
        static const std::string Yellow = "\033[33m";
        static const std::string Blue = "\033[34m";
        static const std::string Magenta = "\033[35m";
        static const std::string Cyan = "\033[36m";
        static const std::string White = "\033[37m";
        
        // 背景色
        static const std::string BgBlack = "\033[40m";
        static const std::string BgRed = "\033[41m";
        static const std::string BgGreen = "\033[42m";
        static const std::string BgYellow = "\033[43m";
        static const std::string BgBlue = "\033[44m";
        static const std::string BgMagenta = "\033[45m";
        static const std::string BgCyan = "\033[46m";
        static const std::string BgWhite = "\033[47m";
    };
#endif

    // 获取对应日志级别的颜色
    inline std::string get_level_color(spdlog::level::level_enum level) {
#ifdef _WIN32
        // Windows 下使用数字代码
        switch (level) {
            case spdlog::level::trace: return "36";    // Cyan
            case spdlog::level::debug: return "32";    // Green
            case spdlog::level::info:  return "37";    // White
            case spdlog::level::warn:  return "33";    // Yellow
            case spdlog::level::err:   return "31";    // Red
            case spdlog::level::critical: return "35"; // Magenta
            default: return "37";
        }
#else
        // Linux/Mac 使用 ANSI 颜色代码
        switch (level) {
            case spdlog::level::trace: return Color::Cyan;
            case spdlog::level::debug: return Color::Green;
            case spdlog::level::info:  return Color::White;
            case spdlog::level::warn:  return Color::Yellow;
            case spdlog::level::err:   return Color::Red;
            case spdlog::level::critical: return Color::Magenta;
            default: return Color::White;
        }
#endif
    }

    // 获取颜色重置代码
    inline std::string get_reset_color() {
#ifdef _WIN32
        return "0";
#else
        return Color::Reset;
#endif
    }

    #define LOG_TRACE(format, ...) InstantSocial::g_logger->trace("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)
    #define LOG_DEBUG(format, ...) InstantSocial::g_logger->debug("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)
    #define LOG_INFO(format, ...) InstantSocial::g_logger->info("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)
    #define LOG_WARN(format, ...) InstantSocial::g_logger->warn("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)
    #define LOG_ERROR(format, ...) InstantSocial::g_logger->error("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)
    #define LOG_FATAL(format, ...) InstantSocial::g_logger->critical("{} {}] " format, InstantSocial::get_filename(__FILE__), __LINE__, ##__VA_ARGS__)

    void init_logger(bool mode, const std::string &file, uint32_t level);
}

#endif