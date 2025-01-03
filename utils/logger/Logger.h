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
    class LoggerFormat;
}

// ����һ�������Ҫ��������std::string��config
template<typename T>
concept IsStringOrLoggerFormat = std::is_same_v<T, std::string> ||
        std::is_same_v<T, utils::LoggerFormat> ||
        std::is_same_v<T, const char *>;

#define FilePath IsStringOrLoggerFormat

namespace utils {

    // ������־ϵͳ�����ã���ֹ�ڸ�ʽ��
    class LoggerFormat {
    public:
        LoggerFormat() = default;
        LoggerFormat(std::string _identifier, std::string _content) : identifier(std::move(_identifier)), content(std::move(_content)) {}

        [[nodiscard]] std::string ToString() const;

        bool operator==(const LoggerFormat & other) const {
            return identifier == other.identifier;
        }

        bool operator!=(const LoggerFormat & other) const {
            return identifier != other.identifier;
        }

        bool operator==(FilePath auto other) const {
            if (std::is_same_v<LoggerFormat, decltype(other)>) {
                return other == *this;
            }
            return false;
        }

    private:
        std::string identifier = "unique";
        std::string content = "format"; // ʵ�ʴ�ӡ������, ��ʼ��������ʶ
    };

    extern const LoggerFormat endl;
    extern const LoggerFormat origin; // ����ԭ�и�ʽ
}

// CRTP, ����̳�ģ����ʽ
class Logger final : public Singleton<Logger> {
public:
    ~Logger() override;

private:
    // ȫ����־�ļ��Ĵ洢
    static std::string LOG_FILE;

    // ȫ����־�ļ����ļ��� log
    static std::string LOG_PATH;

    // ����·��
    static std::string getLogFileFullPath() {
        return LOG_PATH + "/" + LOG_FILE;
    }

    bool showTime = true; // log into time
    std::ofstream outFile;
    // ������(�̰߳�ȫ)
    std::mutex mtx;
    std::mutex endlMtx;

public:
    Logger& operator << (const std::string& message) const;

    Logger &operator <<(const utils::LoggerFormat& format) const;

    // �µ���־·���Լ�����
    void static changeFilePath(FilePath auto format, FilePath auto name);

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

    static std::shared_ptr<Logger> getInstance();

    // Ĭ��׷��ģʽ������ȫ�������־�ļ�
    void clearContent();

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

void Logger::changeFilePath(IsStringOrLoggerFormat auto format, IsStringOrLoggerFormat auto name) {
    bool flag = false;
    if (std::is_same_v<utils::LoggerFormat, decltype(format)>) {
        if (format != utils::origin) {
            std::cerr << "invalid parameters" << std::endl;
            LOG_PATH = "log";
        }
    } else {
        LOG_PATH = name;
        flag = true;
    }
    if (std::is_same_v<utils::LoggerFormat, decltype(name)>) {
        if (name != utils::origin) {
            std::cerr << "invalid parameters" << std::endl;
            LOG_FILE = "log.log";
        }
    } else {
        LOG_FILE = name;
        flag = true;
    }

    if (!flag) {
        return;;
    }

    if (getInstance()->outFile.is_open()) {
        getInstance()->outFile.close();
    }
    getInstance()->start();

}


// ����꣬����ʹ��
#ifndef LOG_GLOBAL
#define LOG *Logger::getInstance()
#define LOG_LINE(_) (*Logger::getInstance()).writeLine(_)
#define LOG_CLS() (*Logger::getInstance()).clearContent()
#define LOG_PATH(a, b) Logger::changeFilePath(a, b)
#endif //LOG_GLOBAL

#endif //LOGGER_H
