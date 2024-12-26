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

// �������������Ϳ��Ա� std::to_string ����
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
        std::string content = "format"; // ʵ�ʴ�ӡ������, ��ʼ��������ʶ
    };

    extern const LoggerFormat endl;
}

// CRTP, ����̳�ģ����ʽ
class Logger : public Singleton<Logger> {
public:
    ~Logger();

private:
    // ȫ����־�ļ��Ĵ洢
    const static char* LOG_FILE;

    // ȫ����־�ļ����ļ��� log
    const static char* LOG_PATH;

    bool showTime = true; // log into time
    std::ofstream outFile;
    // ������(�̰߳�ȫ)
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

    // Ĭ��׷��ģʽ������ȫ�������־�ļ�
    void clearContent() {
        outFile.close();
        outFile.open(LOG_FILE);
        if (!outFile.is_open()) {
            std::cerr << "cannot open log file." << std::endl;
        }
    }
private:
    // ����־�ļ� ��
    void start();

    // �ر���־�ļ� ��
    void stop();

    /**
     * ����־�ļ���Ϣ�洢���ļ���
     * @param message const std::string&
     */
    void info(const std::string& message);
};

// ����꣬����ʹ��
#ifndef LOG_GLOBAL
#define LOG *Logger::getInstance()
#define LOG_LINE(_) (*Logger::getInstance()).writeLine(_)
#define LOG_CLS() (*Logger::getInstance()).clearContent()
#endif //LOG_GLOBAL

#endif //LOGGER_H
