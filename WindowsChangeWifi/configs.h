//
// Created by 86158 on 2024/11/28.
//

#ifndef CONFIGS_H
#define CONFIGS_H

#include "../utils/Singleton.h"

#include <windows.h>

namespace windows::wifi {

    class configs final : public Singleton<configs> {
    public:
        ~configs() override {
            std::cout << "configs desturcted." << std::endl;
        }

        // set basic configs of the application of WindowsChangeWifi
        void applyConfigs() const;

        // set the encoding being UTF8 as default.
        void setUtf8Encoded();

        /**
         * If flag is true, the encoding will be UTF8.
         * Else, it will be not. (ANSI)
         *
         * @param flag bool
         */
        void setUtf8Encoded(bool flag);

        // set needEnterPassword = flag
        void setPasswordEnteredNeed(bool flag);
        [[nodiscard]] bool getPasswordEnteredNeed() const;

        // set needEnterAccount = flag
        void setAccountEnteredNeed(bool flag);
        [[nodiscard]] bool getAccountdEnteredNeed() const;

        [[nodiscard]] LPCWSTR toLPCWSTR(const std::string& str) const;
    private:
        bool utf8Encoded = true;
        bool needEnterAccount = false;
        bool needEnterPassword = false;
    };

    // winerror.h 的错误消息字符串化
    std::wstring GetErrorMessage(DWORD errorCode);

    std::string createProfileXml(const std::string &ssid, const std::string &password, const std::string &account = "");
}

#endif //CONFIGS_H
