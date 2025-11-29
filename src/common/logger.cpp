#include "logger.h"

namespace InstantSocial
{
    std::shared_ptr<spdlog::logger> g_logger;
    
#ifdef _WIN32
    // Windows 颜色控制实现
    #include <windows.h>
    
    void WindowsColor::setColor(int level) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = FOREGROUND_INTENSITY; // 默认高亮
        
        switch (level) {
            case 0: color |= FOREGROUND_BLUE | FOREGROUND_GREEN; break;  // TRACE: Cyan
            case 1: color |= FOREGROUND_GREEN; break;                    // DEBUG: Green
            case 2: color |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break; // INFO: White
            case 3: color |= FOREGROUND_RED | FOREGROUND_GREEN; break;   // WARN: Yellow
            case 4: color |= FOREGROUND_RED; break;                     // ERROR: Red
            case 5: color |= FOREGROUND_RED | FOREGROUND_BLUE; break;    // CRITICAL: Magenta
            default: color |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        }
        
        SetConsoleTextAttribute(hConsole, color);
    }
    
    void WindowsColor::resetColor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
#endif

    void init_logger(bool mode, const std::string &file, uint32_t level)
    {
        if (mode == false) 
        {
            // 控制台输出 - 带颜色
            g_logger = spdlog::stdout_color_mt("default-logger");
            g_logger->set_level(spdlog::level::level_enum::trace);
            g_logger->flush_on(spdlog::level::level_enum::trace);
            
            // 设置带颜色的模式
#ifdef _WIN32
            // Windows 下使用简单的颜色模式
            g_logger->set_pattern("[%H:%M:%S.%e] [%l] [%t] %v");
#else
            // Linux/Mac 下使用 ANSI 颜色
            g_logger->set_pattern("[%H:%M:%S.%e] [%^%l%$] [%t] %v");
#endif
        }
        else 
        {
            // 文件输出 - 无颜色
            g_logger = spdlog::basic_logger_mt("default-logger", file);
            g_logger->set_level((spdlog::level::level_enum)level);
            g_logger->flush_on((spdlog::level::level_enum)level);
            
            // 文件输出使用简单模式，不包含颜色代码
            g_logger->set_pattern("[%H:%M:%S.%e] [%l] [%t] %v");
        }
    }
}