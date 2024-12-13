cmake_minimum_required(VERSION 3.26)
project(WindowsThemeChange)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(WindowsThemeChange WindowsThemeChange/main.cpp
        utils/logger/Logger.cpp
        utils/logger/Logger.h
        utils/myDate/myDate.cpp
        utils/myDate/myDate.h
)

add_executable(WindowsChangeWifi WindowsChangeWifi/main.cpp
        WindowsChangeWifi/utils/CDefer.cpp
        WindowsChangeWifi/utils/CDefer.h
        WindowsChangeWifi/configs.cpp
        WindowsChangeWifi/configs.h
        WindowsChangeWifi/ApSsidInstance.cpp
        WindowsChangeWifi/ApSsidInstance.h
        WindowsChangeWifi/SelfWifi.cpp
        WindowsChangeWifi/SelfWifi.h
)
target_link_libraries(WindowsChangeWifi wlanapi)

