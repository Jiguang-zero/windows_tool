//
// Created by 86158 on 2024/11/28.
//

#ifndef C_DEFER_H
#define C_DEFER_H

#include "functional"

namespace windows::utils {
    /**
     * defer in C++.
     *
     * management resource.
     */
    class CDefer {
    public:
        // We will create an object which can run destructor automatically.
        // RAII
        CDefer(const std::function<void()>& constructor, const std::function<void()>& destructor);

        ~CDefer();

        // delete Copy Constructor and Copy Assignment Operator
        CDefer(const CDefer&) = delete;
        CDefer & operator = (const CDefer& ) = delete;
        CDefer operator = (CDefer) = delete;
    private:
        std::function<void()> _constructor;
        std::function<void()> _destructor;
    };

}

#endif //C_DEFER_H
