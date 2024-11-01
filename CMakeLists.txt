cmake_minimum_required(VERSION 3.26)
project(WindowsThemeChange)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(WindowsThemeChange main.cpp
        utils/logger/Logger.cpp
        utils/logger/Logger.h
        utils/myDate/myDate.cpp
        utils/myDate/myDate.h)

