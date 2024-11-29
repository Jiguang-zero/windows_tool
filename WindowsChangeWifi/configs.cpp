//
// Created by 86158 on 2024/11/28.
//

#include "configs.h"
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
}
