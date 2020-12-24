#ifndef PSS_CONSOLE_OUTPUT_H
#define PSS_CONSOLE_OUTPUT_H

//屏幕输出管控
//add by freeyes

//这里使用第三方的格式化代码
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "singleton.h"

namespace spd = spdlog;

//定义操作宏
#define PSS_LOGGER_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), __VA_ARGS__);
#define PSS_LOGGER_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger(), __VA_ARGS__);
#define PSS_LOGGER_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger(), __VA_ARGS__);
#define PSS_LOGGER_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger(), __VA_ARGS__);

//输出配置结构
class Console_Output_Info
{
public:
    bool m_blTunOn               = true;              //true为屏幕输出, false为文件输出
    int  m_nLogFileMaxSize       = 1024000;           
    int  m_nFileCount            = 0;
    std::string m_strConsoleName = "./consoleoutput";
    std::string m_strLevel       = "info";
};

//输出函数类
class CConsoleOutput
{
public:
    CConsoleOutput() = default;

    void Init(Console_Output_Info obj_Console_Output_Info)
    {
        spdlog::drop_all();

        m_Console_Output_Info = obj_Console_Output_Info;

        std::shared_ptr<spdlog::logger> console = nullptr;
        if (m_Console_Output_Info.m_blTunOn)
        {
            //屏幕输出
            console = spdlog::stdout_logger_mt("console");
        }
        else
        {
            //文件输出
            console = spdlog::rotating_logger_mt("console",
                m_Console_Output_Info.m_strConsoleName.c_str(), 
                m_Console_Output_Info.m_nLogFileMaxSize,
                m_Console_Output_Info.m_nFileCount);
        }

        //设置输出抬头
        console->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%@] %v");
        
        spd::level::level_enum console_level = spdlog::level::info;
        //设置输出等级
        if (m_Console_Output_Info.m_strLevel == "info")
        {
            console_level = spdlog::level::info;
        }
        else if (m_Console_Output_Info.m_strLevel == "debug")
        {
            console_level = spdlog::level::debug;
        }
        else if (m_Console_Output_Info.m_strLevel == "warn")
        {
            console_level = spdlog::level::warn;
        }
        else if (m_Console_Output_Info.m_strLevel == "error")
        {
            console_level = spdlog::level::err;
        }

        console->set_level(console_level);
        console->flush_on(console_level);
        spdlog::set_default_logger(console);
    };

private:
    Console_Output_Info m_Console_Output_Info;
};
using app_ConsoleOutput = PSS_singleton<CConsoleOutput>;


#endif
