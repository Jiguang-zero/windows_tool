//
// Created by 86158 on 2024/11/28.
//

#ifndef CONFIGS_H
#define CONFIGS_H

#include "../utils/Singleton.h"

namespace windows::wifi {

    class configs : Singleton<configs> {
    public:
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

    private:
        bool utf8Encoded = true;
    };

}

#endif //CONFIGS_H
