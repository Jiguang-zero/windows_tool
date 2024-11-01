//
// Created by 86158 on 2024/11/1.
//

#ifndef SINGLETON_H
#define SINGLETON_H

// Singleton.h

#include <memory>
#include <iostream>
#include <mutex>

template <typename T>
class Singleton {
protected:
    Singleton() = default; // 需要被子类采用

    static std::shared_ptr<T> _instance; // #include <memory>
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static std::shared_ptr<T> getInstance() {
        static std::once_flag s_flag; // #include <mutex>
        std::call_once(s_flag, [&]() {
            _instance = std::make_shared<T>();
        });

        return _instance;
    }
    void printAddress() {
        std::cout << _instance.get() << std::endl;
    }

    // 需要被继承
    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif //SINGLETON_H
