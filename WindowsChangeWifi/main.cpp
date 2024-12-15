//
// Created by 86158 on 2024/11/25.
//

#include "ApSsidInstance.h"
#include "configs.h"

// #pragma comment(lib, "wlanapi.lib")
// #pragma comment(lib, "ole32.lib")

int wmain()
{
    windows::wifi::configs::getInstance()->applyConfigs();
    while (true)
    {
        windows::wifi::ApSsidInstance::getInstance()->GetSsidList();
        int choice;
        std::cout << "1: break." << std::endl;
        std::cout << "2: show." << std::endl;
        std::cout << "3: continue." << std::endl;
        std::cout << "4: connect." << std::endl;
        std::cout << "Please enter your choice" << std::endl;
        while (std::cin >> choice) {
            if (choice == 2) {
                windows::wifi::ApSsidInstance::getInstance()->showSsidListWithIndex();
            } else if (choice == 4) {
                int idx;
                std::cin >> idx;;
                windows::wifi::ApSsidInstance::getInstance()->connectToWifi(idx);
            }
            if (choice < 1 || choice > 4) {
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
