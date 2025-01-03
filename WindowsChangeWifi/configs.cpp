//
// Created by 86158 on 2024/11/28.
//

#include "configs.h"
#include "../utils/logger/Logger.h"

#include <windows.h>

namespace windows::wifi {
    void configs::applyConfigs() const {
        if (utf8Encoded) {
            SetConsoleOutputCP(CP_UTF8);
        } else {
            SetConsoleOutputCP(CP_ACP); // ANSI
        }
    }

    void configs::setUtf8Encoded() {
        utf8Encoded = true; // default value
    }

    void configs::setUtf8Encoded(const bool flag) {
        utf8Encoded = flag;
    }

    void configs::setPasswordEnteredNeed(const bool flag) {
        needEnterPassword =flag;
    }

    bool configs::getPasswordEnteredNeed() const {
        return needEnterPassword;
    }

    void configs::setAccountEnteredNeed(const bool flag) {
        needEnterAccount = flag;
    }

    bool configs::getAccountdEnteredNeed() const {
        return needEnterAccount;
    }

    LPCWSTR configs::toLPCWSTR(const std::string &str) const {
        LOG << "str size: " << str.size() << utils::endl;
        int size;
        if (utf8Encoded) {
            size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        } else {
            size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
        }
        LOG << "new size: " << size << utils::endl;
        auto* buffer = new wchar_t[size];
        const auto strP = str.c_str();
        LOG << "strP: " << strP << utils::endl;
        LOG << "*strP: " << *strP << utils::endl;
        int convertedSize;
        if (utf8Encoded) {
            convertedSize = MultiByteToWideChar(CP_UTF8, 0, strP, -1, buffer, size);
        } else {
            convertedSize = MultiByteToWideChar(CP_ACP, 0, strP, -1, buffer, size);
        }
        LOG << "convertedSize: " << convertedSize << utils::endl;
        LOG << "*buffer: " << *buffer << utils::endl;
        return buffer;
    }

    std::wstring GetErrorMessage(const DWORD errorCode) {
        wchar_t* messageBuffer = nullptr;

        // 使用 FormatMessageW 获取宽字符的错误字符串
        const DWORD size = FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 使用默认语言
            reinterpret_cast<LPWSTR>(&messageBuffer),  // 自动分配内存
            0,
            nullptr
        );

        std::wstring errorMessage;
        if (size > 0 && messageBuffer) {
            errorMessage = messageBuffer; // 转换为 std::wstring
        } else {
            errorMessage.clear(); // 空白
        }

        // 释放分配的内存
        if (messageBuffer) {
            LocalFree(messageBuffer);
        }

        return errorMessage;
    }

    std::string createProfileXml(const std::string &ssid, const std::string &password, const std::string &account) {
        if (account.empty()) {
            // 使用预共享密钥 (WPA2-PSK) 的配置文件
            return R"(<WLANProfile xmlns="http://www.microsoft.com/networking/WLAN/profile/v1">
        <name>)" + ssid + R"(</name>
        <SSIDConfig>
            <SSID>
                <name>)" + ssid + R"(</name>
            </SSID>
        </SSIDConfig>
        <connectionType>ESS</connectionType>
        <connectionMode>auto</connectionMode>
        <MSM>
            <security>
                <authEncryption>
                    <authentication>WPA2PSK</authentication>
                    <encryption>AES</encryption>
                    <useOneX>false</useOneX>
                </authEncryption>
                <sharedKey>
                    <keyType>passPhrase</keyType>
                    <protected>false</protected>
                    <keyMaterial>)" + password + R"(</keyMaterial>
                </sharedKey>
            </security>
        </MSM>
    </WLANProfile>)";
        }

        // 使用账号和密码的配置文件（WPA2-Enterprise）
        return R"(<WLANProfile xmlns="http://www.microsoft.com/networking/WLAN/profile/v1">
            <name>)" + ssid + R"(</name>
            <SSIDConfig>
                <SSID>
                    <name>)" + ssid + R"(</name>
                </SSID>
            </SSIDConfig>
            <connectionType>ESS</connectionType>
            <connectionMode>auto</connectionMode>
            <MSM>
                <security>
                    <authEncryption>
                        <authentication>WPA2</authentication>
                        <encryption>AES</encryption>
                        <useOneX>true</useOneX>
                    </authEncryption>
                    <OneX xmlns="http://www.microsoft.com/networking/OneX/v1">
                        <authMode>user</authMode>
                        <EAPConfig>
                            <EapHostConfig xmlns="http://www.microsoft.com/provisioning/EapHostConfig">
                                <EapMethod>
                                    <Type xmlns="http://www.microsoft.com/networking/EapMethods/1.0">25</Type> <!-- PEAP -->
                                    <VendorId xmlns="http://www.microsoft.com/networking/EapMethods/1.0">0</VendorId>
                                    <VendorType xmlns="http://www.microsoft.com/networking/EapMethods/1.0">0</VendorType>
                                    <AuthorId xmlns="http://www.microsoft.com/networking/EapMethods/1.0">0</AuthorId>
                                </EapMethod>
                                <Config xmlns="http://www.microsoft.com/networking/EapHostConfig">
                                    <Eap xmlns="http://www.microsoft.com/networking/EapMethods/PEAP/0">
                                        <Type>25</Type> <!-- PEAP -->
                                        <EapType xmlns="http://www.microsoft.com/networking/EapMethods/EapTtls/0">
                                            <Type>13</Type> <!-- MSCHAPv2 -->
                                            <CredentialsSource>
                                                <UserName>)" + account + R"(</UserName>
                                                <Password>)" + password + R"(</Password>
                                            </CredentialsSource>
                                        </EapType>
                                        <EnableQuarantineChecks>false</EnableQuarantineChecks>
                                    </Eap>
                                </Config>
                            </EapHostConfig>
                        </EAPConfig>
                    </OneX>
                </security>
            </MSM>
        </WLANProfile>)";
    }
}
