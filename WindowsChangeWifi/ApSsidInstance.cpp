//
// Created by 86158 on 2024/11/29.
//

#include "ApSsidInstance.h"

namespace windows::wifi {
    bool ApSsidInstance::InitialHandle() {
        DWORD dwCurVersion = 0;
        if (wlanHandle == nullptr) {
            DWORD dwResult;
            if (constexpr DWORD dwMaxClient = 2;
                (dwResult = WlanOpenHandle(dwMaxClient, nullptr, &dwCurVersion, &wlanHandle)) != ERROR_SUCCESS)
            {
                std::cout << "wlanOpenHandle failed with error: " << dwResult << std::endl;
                wlanHandle = nullptr;
                return false;
            }
            // 注册消息通知回调
            if ((dwResult = WlanRegisterNotification(wlanHandle, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, OnNotificationCallback, nullptr, nullptr, nullptr)) != ERROR_SUCCESS)
            {
                std::cout << "wlanRegisterNotification failed with error: " << dwResult << std::endl;
                wlanHandle = nullptr;
                return false;
            }
        }
        return true;
    }

    void ApSsidInstance::CvNotify() {
        cv.notify_all();
    }

}