//
// Created by 86158 on 2024/11/28.
//

#include "CDefer.h"

namespace windows::utils {
    CDefer::CDefer(const std::function<void()> &constructor, const std::function<void()> &destructor)
        : _constructor(constructor), _destructor(destructor) {
        if (_constructor) {
            _constructor();
        }
    }

    CDefer::~CDefer() {
        if (_destructor) {
            _destructor();
        }
    }
}
