//
// Created by 86158 on 2024/12/13.
//

#ifndef SELF_WIFI_H
#define SELF_WIFI_H

#include <string>

namespace windows::wifi {

class SelfWifi {
public:
    // default constructor, and quality will be 0 as default.
    explicit SelfWifi(std::string _name);

    SelfWifi(std::string _name, int _quality);

    // we do not need to use them.
    SelfWifi(const SelfWifi&) = delete;
    SelfWifi& operator = (const SelfWifi&) = delete;
    SelfWifi& operator = (const SelfWifi&&) = delete; // rvalue reference

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int getQuality() const;

    bool operator == (const SelfWifi& other) const;
    bool operator < (const SelfWifi& other) const;
    bool operator > (const SelfWifi& other) const;
    bool operator <= (const SelfWifi& other) const;
    bool operator >= (const SelfWifi& other) const;

private:
    std::string name;
    int quality; // the Wi-Fi quality, and the higher the value, the better the signal

    // set quality.
    // should not be used outside.
    [[maybe_unused]] void setQuality(int _quality);
};

} // wifi
// windows

#endif //SELF_WIFI_H
