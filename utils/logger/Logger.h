//
// Created by 86158 on 2024/11/1.
//

#ifndef LOGGER_H
#define LOGGER_H

// Logger.h
#include "../Singleton.h"

#include <fstream>

// CRTP, 奇异继承模板样式
class Logger : public Singleton<Logger> {
public:
    ~Logger();

private:
    // 全局日志文件的存储
    const static char* LOG_FILE;

    // 全局日志文件的文件夹 log
    const static char* LOG_PATH;

    std::ofstream outFile;
    // 互斥锁(线程安全)
    std::mutex mtx;

public:
    std::shared_ptr<Logger> &operator << (const std::string &message) const {
        _instance->info(message);

        return _instance;
    }

    static std::shared_ptr<Logger> getInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = std::make_shared<Logger>();
            _instance->start();
        });

        return _instance;
    }
private:
    // 打开日志文件 流
    void start();

    // 关闭日志文件 流
    void stop();

    /**
     * 将日志文件消息存储到文件中
     * @param message const std::string&
     */
    void info(const std::string& message);

};

// 定义宏，方便使用
#ifndef LOG_GLOBAL
#define LOG *Logger::getInstance()
#endif //LOG_GLOBAL

#endif //LOGGER_H
