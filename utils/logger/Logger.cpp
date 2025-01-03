//
// Created by 86158 on 2024/11/1.
//

// Logger.cpp
#include "Logger.h"

#include <sstream>
#include <filesystem>

#include "../myDate/myDate.h"

std::string Logger::LOG_FILE = "log.log";
std::string Logger::LOG_PATH = "log";

namespace utils {
    const LoggerFormat endl("endl", "\n");
    const LoggerFormat origin("origin", "");
}

std::string getFullTimeForLog() {
    std::ostringstream oss;

    const myDate date;

    oss << std::to_string(date.getYear()) + ".";
    oss << std::to_string(date.getMonth()) + ".";
    oss << std::to_string(date.getDay()) + " ";

    oss << std::to_string(date.getHour()) + ":";
    oss << std::to_string(date.getMinute()) + ":";
    oss << std::to_string(date.getSecond()) + " ";

    oss << date.getDow();

    return oss.str();
}

void Logger::info(const std::string &message) {

    std::unique_lock lock(mtx);
    {
        if (!outFile.is_open()) {
            start();
        }
        std::unique_lock lock1(endlMtx);
        if (showTime) {
            outFile << "[" << getFullTimeForLog() << "]";
            showTime = false;
        }
        outFile << " " << message;
        stop();
    }
}

std::string utils::LoggerFormat::ToString() const {
    return content;
}

Logger::~Logger() {
    std::cout << "Logger destructed" << std::endl;
    stop();
}

Logger & Logger::operator<<(const std::string &message) const {
    _instance->info(message);

    return *_instance;
}

Logger & Logger::operator<<(const utils::LoggerFormat& format) const {
    const auto str = format.ToString();
    _instance->info(str);
    if (str == "\n") {
        std::unique_lock lock(_instance->endlMtx);
        _instance->showTime = true;
    }
    return *_instance;
}

void Logger::writeLine(const std::string &message) {
    _instance->info(message);
    _instance->info("\n");
    showTime = true;
}

std::shared_ptr<Logger> Logger::getInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
        _instance = std::make_shared<Logger>();
        _instance->start();
    });

    return _instance;
}

void Logger::clearContent() {
    outFile.close();
    outFile.open(getLogFileFullPath());
    if (!outFile.is_open()) {
        std::cerr << "cannot open log file." << std::endl;
    }
}

void Logger::start() {
    // 如果不存在 log 文件夹，则创建
    if (const std::filesystem::path folderPath = LOG_PATH; !exists(folderPath)) {
        if (!create_directories(folderPath)) {
            std::cerr << "create log folder error." << std::endl;
            // exit(-1)
        }
    }

    // outFile.open(LOG_FILE); // debug模式下不追加
    // 打开文件输出流 追加模式
    outFile.open(getLogFileFullPath(), std::ofstream::app);
    if (!outFile.is_open()) {
        std::cerr << "log/log.txt is not open." << std::endl;
    }
}

void Logger::stop() {
    outFile.close();
    if (outFile.is_open()) {
        std::cerr << "log/log.txt close failed." << std::endl;
    }
}
