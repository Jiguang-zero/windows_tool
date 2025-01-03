//
// Created by 86158 on 2024/11/25.
//

#include "ApSsidInstance.h"
#include "configs.h"
#include "../utils/logger/Logger.h"

int main(const int argc, char** argv)
{
    const auto config = windows::wifi::configs::getInstance();
    config->applyConfigs();
    // argv[1] == "-p"
    // We may support account, but we don't test it.
    if (argc == 2 && strcmp(argv[1], "-p") == 0) {
        std::cout << "Please note that you should enter password when you want to connect WiFi." << std::endl;
        std::cout << "And this may cover the profile." << std::endl;
        config->setPasswordEnteredNeed(true);
    } else if (argc == 1) {
        config->setPasswordEnteredNeed(false);
    } else {
        std::cout << "Please Run this application with no argcs for better."<< std::endl;
        std::cout << "If you run the application with \"-p\", "
                     "please note that every Wi-Fi you change, you should input the password." << std::endl;
        std::cout << "And please remember that you cannot set WiFi with Chinse-character ssid." << std::endl;

        return -1;
    }

    LOG_PATH(utils::origin, "wifi.log");
    // Here we don't clear the log.
    // LOG_CLS();

    while (true)
    {
        int choice;
        std::cout << "1: break." << std::endl;
        std::cout << "2: show." << std::endl;
        std::cout << "3: scan." << std::endl;
        std::cout << "4: connect." << std::endl;
        std::cout << "5: set password needed." << std::endl;
        std::cout << "Please enter your choice" << std::endl;
        while (std::cin >> choice) {
            if (choice == 2) {
                LOG << "show list" << utils::endl;
                windows::wifi::ApSsidInstance::getInstance()->showSsidListWithIndex();
            } else if (choice == 4) {
                LOG_LINE("try to connect");
                int idx;
                std::cin >> idx;;
                windows::wifi::ApSsidInstance::getInstance()->connectToWifi(idx);
            } else if (choice == 3) {
                windows::wifi::ApSsidInstance::getInstance()->GetSsidList();
                LOG_LINE("scan");
            } else if (choice == 5) {
                auto needed = config->getPasswordEnteredNeed();
                needed = !needed;
                config->setPasswordEnteredNeed(needed);
                std::cout << "Password Needed is " << std::to_string(needed) << std::endl;
            }
            if (choice < 1 || choice > 5) {
                std::cout << "Please enter your choice" << std::endl;
                continue;
            }
            break;
        }
        if (choice == 1) {
            break;
        }
    }

    return 0;
}
