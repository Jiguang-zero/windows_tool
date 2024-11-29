//
// Created by 86158 on 2024/11/25.
//

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
#include <functional>

#include "utils/CDefer.h"

using windows::utils::CDefer;

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

class ApSsidInstance
{
    friend void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);
public:
    static ApSsidInstance& GetInstance()
    {
        static ApSsidInstance instance;
        return instance;
    }
    bool GetSsidList(std::vector<std::string>& wifilist);
    void CvNotify();
private:
    ApSsidInstance()
        : _wlanHandle(NULL)
    {
    }

    bool InitialHandle();
private:
    HANDLE _wlanHandle;
    std::vector<std::string> _ssidList;
    std::condition_variable _cv;
    std::mutex _mutex;
};

void  OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);

int wmain()
{
    while (true)
    {
        std::vector<std::string> wifis;
        ApSsidInstance::GetInstance().GetSsidList(wifis);
        Sleep(100);
    }

    while (true) std::this_thread::sleep_for(std::chrono::hours(10));
    return 0;
}

bool ApSsidInstance::GetSsidList(std::vector<std::string>& wifilist)
{
    bool result = false;
    if (!InitialHandle())
    {
        std::cout << "initial wlan handle failed" << std::endl;
        return false;
    }

    DWORD dwResult = 0;
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;
    WCHAR GuidString[39] = { 0 };

    PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
    PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

    SetConsoleOutputCP(CP_UTF8);

    // 释放内存
    CDefer pIfRaii([]() {}, [&pBssList, &pIfList]()
                     {
                         if (pBssList != NULL)
                         {
                             WlanFreeMemory(pBssList);
                             pBssList = NULL;
                         }

                         if (pIfList != NULL)
                         {
                             WlanFreeMemory(pIfList);
                             pIfList = NULL;
                         }
                     });

    unsigned int i, j;

    dwResult = WlanEnumInterfaces(_wlanHandle, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS)
    {
        return false;
    }
    else
    {
        for (i = 0; i < (int)pIfList->dwNumberOfItems; i++)
        {
            pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];

            dwResult = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString,
                                       sizeof(GuidString) / sizeof(*GuidString));
            // 向无线网卡发送探测请求
            dwResult = WlanScan(_wlanHandle, (const GUID*)(&pIfInfo->InterfaceGuid), NULL, NULL, NULL);
            if (dwResult != ERROR_SUCCESS)
            {
                return false;
            }
        }
    }

    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait_for(lock, std::chrono::seconds(4));
    }

    for (i = 0; i < (int)pIfList->dwNumberOfItems; i++)
    {
        pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];
        dwResult = WlanGetAvailableNetworkList(_wlanHandle,
                                               &pIfInfo->InterfaceGuid,
                                               0,
                                               NULL,
                                               &pBssList);

        if (dwResult != ERROR_SUCCESS)
        {
            std::cout << "WlanGetAvailableNetworkList failed with error:" << dwResult;
            return false;
        }
        for (j = 0; j < pBssList->dwNumberOfItems; j++)
        {
            pBssEntry = (WLAN_AVAILABLE_NETWORK*)&pBssList->Network[j];

            std::string temp = std::string(reinterpret_cast<char*>(pBssEntry->dot11Ssid.ucSSID));
            if (temp.find("BugM") != std::string::npos)
            {
                std::cout << "+++++++++++++++++" << std::endl;
            }
            wifilist.emplace_back(temp);
            std::cout << "wifi name:" << temp << std::endl;
        }
    }

    return true;
}

void ApSsidInstance::CvNotify()
{
    _cv.notify_all();
}

bool ApSsidInstance::InitialHandle()
{
    DWORD dwResult = 0;
    DWORD dwCurVersion = 0;
    DWORD dwMaxClient = 2;
    if (_wlanHandle == NULL)
    {
        if ((dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &_wlanHandle)) != ERROR_SUCCESS)
        {
            std::cout << "wlanOpenHandle failed with error: " << dwResult << std::endl;
            _wlanHandle = NULL;
            return false;
        }
        // 注册消息通知回调
        if ((dwResult = WlanRegisterNotification(_wlanHandle, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, WLAN_NOTIFICATION_CALLBACK(OnNotificationCallback), NULL, nullptr, nullptr)) != ERROR_SUCCESS)
        {
            std::cout << "wlanRegisterNotification failed with error: " << dwResult << std::endl;
            _wlanHandle = NULL;
            return false;
        }
    }
    return true;
}

void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context)
{
    if (data != NULL && data->NotificationSource == WLAN_NOTIFICATION_SOURCE_ACM && (data->NotificationCode == wlan_notification_acm_scan_complete || data->NotificationCode == wlan_notification_acm_start))
    {
        ApSsidInstance::GetInstance().CvNotify();
    }
}