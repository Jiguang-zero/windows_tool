//
// Created by 86158 on 2024/11/1.
//

#ifndef LOGGER_H
#define LOGGER_H

// Logger.h
#include "../Singleton.h"

#include <fstream>
#include <string>
#include <utility>

// 定义概念，限制类型可以被 std::to_string 处理
template <typename T>
concept ConvertibleToString = requires(T value) {
    { std::to_string(value) } -> std::convertible_to<std::string>;
};

namespace utils {
    class LoggerFormat {
    public:
        LoggerFormat() = default;
        explicit LoggerFormat(std::string  _content) : content(std::move(_content)) {}

        [[nodiscard]] std::string ToString() const;

    private:
        std::string content = "format"; // 实际打印的内容, 初始化已做标识
    };

    extern const LoggerFormat endl;
}

// CRTP, 奇异继承模板样式
class Logger : public Singleton<Logger> {
public:
    ~Logger();

private:
    // 全局日志文件的存储
    const static char* LOG_FILE;

    // 全局日志文件的文件夹 log
    const static char* LOG_PATH;

    bool showTime = true; // log into time
    std::ofstream outFile;
    // 互斥锁(线程安全)
    std::mutex mtx;
    std::mutex endlMtx;

public:
    Logger& operator << (const std::string& message) const;

    Logger &operator <<(const utils::LoggerFormat& format) const;

    void writeLine(const std::string& message);

    template <ConvertibleToString T>
    void writeLine(T message) const {
        writeLine(std::to_string(message));
    }

    template <ConvertibleToString T>
    Logger& operator << (T message) const {
        _instance->info(std::to_string(message));

        return *_instance;
    }

    static std::shared_ptr<Logger> getInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            _instance = std::make_shared<Logger>();
            _instance->start();
        });

        return _instance;
    }

    // 默认追加模式，现在全部清空日志文件
    void clearContent() {
        outFile.close();
        outFile.open(LOG_FILE);
        if (!outFile.is_open()) {
            std::cerr << "cannot open log file." << std::endl;
        }
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
#define LOG_LINE(_) (*Logger::getInstance()).writeLine(_)
#define LOG_CLS() (*Logger::getInstance()).clearContent()
#endif //LOG_GLOBAL

#endif //LOGGER_H
