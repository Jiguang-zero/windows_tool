//
// Created by 86158 on 2024/11/29.
//

#ifndef AP_SSID_INSTANCE_H
#define AP_SSID_INSTANCE_H

#include "../utils/Singleton.h"

#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <wlanapi.h>
#include <unordered_map>

namespace windows::wifi {
    void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);

    void freeMemory(PWLAN_AVAILABLE_NETWORK_LIST& pBssList, PWLAN_INTERFACE_INFO_LIST& pIfList);

    class ApSsidInstance : public Singleton<ApSsidInstance> {
    public:
        void CvNotify();

        bool GetSsidList();

        // show ssidList in the format like the the one below.
        // 1: Jiguang
        // 2: Zearo
        void showSsidListWithIndex() const;

    private:
        // ³õÊ¼»¯ÊÖ±ú
        bool InitialHandle();

        HANDLE wlanHandle = nullptr;
        std::mutex mtx;
        std::condition_variable cv;

        // WIFI list
        std::vector<std::string> ssidList;
    };

}

#endif //AP_SSID_INSTANCE_H
