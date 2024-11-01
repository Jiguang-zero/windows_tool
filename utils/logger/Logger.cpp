//
// Created by 86158 on 2024/11/1.
//

// Logger.cpp
#include "Logger.h"

#include <sstream>
#include <filesystem>

#include "../myDate/myDate.h"

const char* Logger::LOG_FILE = "log/log.txt";
const char* Logger::LOG_PATH = "log";

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
    outFile << getFullTimeForLog() << " " << message << std::endl;
}

Logger::~Logger() {
    stop();
}

void Logger::start() {
    // 如果不存在 log 文件夹，则创建
    if (const std::filesystem::path folderPath = LOG_PATH; !exists(folderPath)) {
        if (!create_directories(folderPath)) {
            std::cerr << "create log folder error." << std::endl;
            // exit(-1)
        }
    }

    // 打开文件输出流 追加模式
    outFile.open(LOG_FILE, std::ofstream::app);
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
