//
// Created by 86158 on 2024/12/13.
//

#ifndef SELF_WIFI_H
#define SELF_WIFI_H

#include <iostream>

namespace windows::wifi {

class SelfWifi {
public:
    // default constructor, and quality will be 0 as default.
    explicit SelfWifi(std::string _name);

    SelfWifi(std::string _name, int _quality);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] int getQuality() const;

    bool operator == (const SelfWifi& other) const;
    bool operator < (const SelfWifi& other) const;
    bool operator > (const SelfWifi& other) const;
    bool operator <= (const SelfWifi& other) const;
    bool operator >= (const SelfWifi& other) const;

    friend std::ostream& operator << (std::ostream& os, const SelfWifi& wifi);

private:
    std::string name;
    int quality; // the Wi-Fi quality, and the higher the value, the better the signal

    // set quality.
    // should not be used outside.
    [[maybe_unused]] void setQuality(int _quality);
};

    struct _MaxComparatorSelfWifi { // NOLINT(*-reserved-identifier)
        // left hand side, right hand side
        bool operator()(const SelfWifi& lhs, const SelfWifi& rhs) const {
            return lhs > rhs;
        }
    };

} // wifi
// windows

#endif //SELF_WIFI_H
