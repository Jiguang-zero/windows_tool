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

    // 内部函数类型
    // 当 fn.first == true时，表明调用此函数的循环提前退出或者直接返回fn.second
    using _INTERNAL_APPLY_NETWORK_LIST = std::function<std::pair<bool, bool>(PWLAN_INTERFACE_INFO, PWLAN_AVAILABLE_NETWORK_LIST &)>; // NOLINT(*-reserved-identifier)

    class ApSsidInstance : public Singleton<ApSsidInstance> {
    public:
        // destructor , close handle and so on.
        ~ApSsidInstance();

        void CvNotify();

        /**
         * 由于可能前面扫描与第二次扫描的时间差距比较大，wifi可能被动关闭等等很多情况，所以每次都会初始化手柄，从头开始
         * @return if we scan successfully, it will return true.
         */
        bool GetSsidList();

        /**
         *
         * @param index int, 表示ssid的第i个元素 连接该wifi
         * @return 连接成功返回true
         */
        bool connectToWifi(int index);

        // show ssidList in the format like the one below.
        // 1: Jiguang
        // 2: Zearo
        void showSsidListWithIndex() const;

    private:
        // 初始化手柄
        bool InitialHandle();

        /**
         * 获取接口列表以及可用网络列表, 并执行fn
         * @param pBssList
         * @param pIfList
         * @param fn 如果fn 返回真，那么在循环中会提前退出，否则不会
         */
        bool getNetworkAndInterfaceList(
            PWLAN_AVAILABLE_NETWORK_LIST& pBssList,
            PWLAN_INTERFACE_INFO_LIST& pIfList,
            const _INTERNAL_APPLY_NETWORK_LIST &fn);

        // try to connect to Wi-Fi
        bool connectToWifi(const std::string& targetSsid);

        // connecting to Wi-Fi
        [[nodiscard]] bool connectingWifi(const PWLAN_AVAILABLE_NETWORK_LIST &pNetworkList, const std::string& targetSsid, const PWLAN_INTERFACE_INFO &info) const;

        // 插入到 ssidList set 中
        void insertWifi(const PWLAN_AVAILABLE_NETWORK_LIST& pBssList);

        HANDLE wlanHandle = nullptr;
        std::mutex mtx;
        std::condition_variable cv;

        // WIFI list
        std::set<SelfWifi, _MaxComparatorSelfWifi> ssidList;
    };

}

#endif //AP_SSID_INSTANCE_H
