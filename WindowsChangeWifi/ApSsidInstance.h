//
// Created by 86158 on 2024/11/29.
//

#ifndef AP_SSID_INSTANCE_H
#define AP_SSID_INSTANCE_H

#include "../utils/Singleton.h"
#include "SelfWifi.h"

#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <set>
#include <wlanapi.h>

namespace windows::wifi {
    void OnNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);

    void freeMemory(PWLAN_AVAILABLE_NETWORK_LIST& pBssList, PWLAN_INTERFACE_INFO_LIST& pIfList);

    // The type of function.
    // When fn.first == true, it means that the loop called this function will exit in advance or will return fn.second.
    using _INTERNAL_APPLY_NETWORK_LIST = std::function<std::pair<bool, bool>(PWLAN_INTERFACE_INFO, PWLAN_AVAILABLE_NETWORK_LIST &)>; // NOLINT(*-reserved-identifier)

    class ApSsidInstance final : public Singleton<ApSsidInstance> {
    public:
        // destructor , close handle and so on.
        ~ApSsidInstance() override;

        void CvNotify();

        /**
         * @return if we scan successfully, it will return true.
         */
        bool GetSsidList();

        /**
         *
         * @param index int, ssidList[index]
         * @return It will return ture if we connect it successfully.
         */
        bool connectToWifi(int index);

        // show ssidList in the format like the one below.
        // 1: Jiguang
        // 2: Zearo
        void showSsidListWithIndex() const;

    private:
        // Initial handle
        bool InitialHandle();

        /**
         * Get the interface info list and available network list.
         * And excute the function fn.
         * @param pBssList
         * @param pIfList
         * @param fn If fn returns true, it will exit in advance in the loop.
         */
        bool getNetworkAndInterfaceList(
            PWLAN_AVAILABLE_NETWORK_LIST& pBssList,
            PWLAN_INTERFACE_INFO_LIST& pIfList,
            const _INTERNAL_APPLY_NETWORK_LIST &fn);

        // try to connect to Wi-Fi
        bool connectToWifi(const std::string& targetSsid);

        // connecting to Wi-Fi
        bool connectingWifi(const PWLAN_AVAILABLE_NETWORK_LIST &pNetworkList, const std::string& targetSsid, const PWLAN_INTERFACE_INFO &info);

        // inset into ssidList
        void insertWifi(const PWLAN_AVAILABLE_NETWORK_LIST& pBssList);

        // set profile
        bool setWifiProfile(const PWLAN_INTERFACE_INFO &info, const std::string& profile);

        [[nodiscard]] bool deleteProfile(const PWLAN_INTERFACE_INFO& info, const std::string& ssid) const;

        // check whether the profile exist or not.
        [[nodiscard]] bool profileExisted(const PWLAN_INTERFACE_INFO &info, const std::string& ssid) const;

        HANDLE wlanHandle = nullptr;
        std::mutex mtx;
        std::condition_variable cv;

        // WIFI list
        std::set<SelfWifi, _MaxComparatorSelfWifi> ssidList;
    };

}

#endif //AP_SSID_INSTANCE_H
