//
// Created by 86158 on 2024/11/29.
//

#include "ApSsidInstance.h"
#include "utils/CDefer.h"

#include <iomanip>

namespace windows::wifi {
    bool ApSsidInstance::InitialHandle() {
        DWORD dwCurVersion = 0;
        if (wlanHandle == nullptr) {
            DWORD dwResult;
            if (constexpr DWORD dwMaxClient = 2;
                (dwResult = WlanOpenHandle(dwMaxClient, nullptr,
                    &dwCurVersion, &wlanHandle)) != ERROR_SUCCESS) {
                std::cout << "wlanOpenHandle failed with error: " << dwResult << std::endl;
                wlanHandle = nullptr;
                return false;
            }
            // 注册消息通知回调
            if ((dwResult = WlanRegisterNotification(wlanHandle, WLAN_NOTIFICATION_SOURCE_ALL,
                TRUE, OnNotificationCallback,
                nullptr, nullptr, nullptr)) != ERROR_SUCCESS) {
                std::cout << "wlanRegisterNotification failed with error: " << dwResult << std::endl;
                wlanHandle = nullptr;
                return false;
            }
        }
        return true;
    }

    // ReSharper disable once CppParameterMayBeConst
    void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, [[maybe_unused]] PVOID context) {
        if (data != nullptr && data->NotificationSource == WLAN_NOTIFICATION_SOURCE_ACM &&
            (data->NotificationCode == wlan_notification_acm_scan_complete || data->NotificationCode == wlan_notification_acm_start)) {
            ApSsidInstance::getInstance()->CvNotify();
        }
    }

    void ApSsidInstance::CvNotify() {
        cv.notify_all();
    }

    void freeMemory(PWLAN_AVAILABLE_NETWORK_LIST& pBssList, PWLAN_INTERFACE_INFO_LIST& pIfList) {
        if (pBssList != nullptr) {
            WlanFreeMemory(pBssList);
            pBssList = nullptr;
        }

        if (pIfList != nullptr) {
            WlanFreeMemory(pIfList);
            pIfList = nullptr;
        }
    }

    bool ApSsidInstance::GetSsidList() {
        if (!InitialHandle()) {
            std::cout << "Initialize Wlan Handle Failed." << std::endl;
            return false;
        }
        PWLAN_INTERFACE_INFO_LIST pIfList = nullptr;
        PWLAN_INTERFACE_INFO pIfInfo;
        WCHAR GuidString[39] = {};

        PWLAN_AVAILABLE_NETWORK_LIST pBssList = nullptr;

        // free memory
        utils::CDefer pIfRaii(
            nullptr,
            [&pBssList, &pIfList]() {
                freeMemory(pBssList, pIfList);
            });

        unsigned int i;

        DWORD dwResult = WlanEnumInterfaces(wlanHandle, nullptr, &pIfList);
        if (dwResult != ERROR_SUCCESS) {
            return false;
        }
        for (i = 0; i < pIfList->dwNumberOfItems; i++) {
            pIfInfo = &pIfList->InterfaceInfo[i];

            const int iRet = StringFromGUID2(pIfInfo->InterfaceGuid, reinterpret_cast<LPOLESTR>(&GuidString),
                                             std::size(GuidString));
            if (iRet == 0) {
                std::cout << "String From GUID2 Failed." << std::endl;
                return false;
            }
            // 向无线网卡发送探测请求
            dwResult = WlanScan(wlanHandle, &pIfInfo->InterfaceGuid, nullptr, nullptr, nullptr);
            if (dwResult != ERROR_SUCCESS) {
                return false;
            }
        }

        {
            std::unique_lock lock(mtx);
            cv.wait_for(lock, std::chrono::seconds(4));
        }

        for (i = 0; i < static_cast<int>(pIfList->dwNumberOfItems); i++)
        {
            pIfInfo = &pIfList->InterfaceInfo[i];
            dwResult = WlanGetAvailableNetworkList(wlanHandle,
                                                   &pIfInfo->InterfaceGuid,
                                                   0,
                                                   nullptr,
                                                   &pBssList);

            if (dwResult != ERROR_SUCCESS)
            {
                std::cout << "WlanGetAvailableNetworkList failed with error:" << dwResult;
                return false;
            }
            // clear ssidList.
            ssidList.clear();
            for (unsigned int j = 0; j < pBssList->dwNumberOfItems; j++)
            {
                // ReSharper disable once CppLocalVariableMayBeConst
                PWLAN_AVAILABLE_NETWORK pBssEntry = &pBssList->Network[j];

                if (auto temp = std::string(reinterpret_cast<char*>(pBssEntry->dot11Ssid.ucSSID));
                    !temp.empty() /* && pBssEntry->bNetworkConnectable */) {
                    SelfWifi wifi(temp, static_cast<int>(pBssEntry->wlanSignalQuality));
                    ssidList.insert(wifi);
                }
            }
        }

        return true;
    }

    void ApSsidInstance::showSsidListWithIndex() const {
        const int ssidListNumber = static_cast<int>(ssidList.size());

        std::cout << "There are " << ssidListNumber << " Wifi you can choose" << std::endl;
        int i = 0;
        for (const auto & ssid : ssidList) {
            std::cout << std::right;
            std::cout << std::setw(5) << ++i << ". ";

            std::cout << std::left;
            std::cout << ssid << std::endl;
        }
    }
}

