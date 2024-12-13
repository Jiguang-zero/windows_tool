//
// Created by 86158 on 2024/12/13.
//

#include "SelfWifi.h"

#include <iomanip>
#include <utility>

namespace windows::wifi {
    SelfWifi::SelfWifi(std::string _name) : name(std::move(_name)) {
        quality = 0; // as default, and this should not be connected unless we set the quality.
    }

    SelfWifi::SelfWifi(std::string _name, const int _quality) : name(std::move(_name)), quality(_quality) {
    }

    std::string SelfWifi::getName() const {
        return name;
    }

    int SelfWifi::getQuality() const {
        return quality;
    }

    bool SelfWifi::operator==(const SelfWifi &other) const {
        return other.name == this->name && quality == other.quality;
    }

    bool SelfWifi::operator<(const SelfWifi &other) const {
        return quality < other.quality;
    }

    bool SelfWifi::operator>(const SelfWifi &other) const {
        return quality > other.quality;
    }

    bool SelfWifi::operator<=(const SelfWifi &other) const {
        return this->operator==(other) || this->operator<(other);
    }

    bool SelfWifi::operator>=(const SelfWifi &other) const {
        return this->operator==(other) || this->operator>(other);
    }

    void SelfWifi::setQuality(const int _quality) {
        quality = _quality;
    }

    std::ostream & operator<<(std::ostream & os, const SelfWifi &wifi) {
        os << "(" << wifi.getQuality() << "%)" << std::setw(4) << " ";
        os << std::setw(30) << wifi.getName() << " ";

        return os;
    }
}
