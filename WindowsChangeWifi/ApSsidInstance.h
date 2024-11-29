//
// Created by 86158 on 2024/11/29.
//

#ifndef AP_SSID_INSTANCE_H
#define AP_SSID_INSTANCE_H

#include "../utils/Singleton.h"

#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <wlanapi.h>

namespace windows::wifi {
    void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);

    class ApSsidInstance : Singleton<ApSsidInstance> {
    public:
        void CvNotify();

    private:
        bool InitialHandle();

        HANDLE wlanHandle = nullptr;
        std::mutex mtx;
        std::condition_variable cv;
        std::vector<std::string> ssidList;
    };

}

#endif //AP_SSID_INSTANCE_H
