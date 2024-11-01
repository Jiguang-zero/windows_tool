//
// Created by 86158 on 2024/11/1.
//

#ifndef MY_DATE_H
#define MY_DATE_H

#include <string>

class myDate {
public:
    /**
     * Tomohiko Sakamoto 算法，获取当前日期是星期几 (1582-10-15 之后)
     * @param year
     * @param month
     * @param day
     * @return int
     */
    static int dow(int year, int month, int day);

    /**
     * 如果输出数字错误，返回 空
     * @param day 星期几 (0~6), 如果为0,就是星期天
     * @return 星期几的英文
     */
    static std::string getDowString(int day);

    myDate();

private:
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;

public:
    [[nodiscard]] int getYear() const;
    [[nodiscard]] int getMonth() const;
    [[nodiscard]] int getDay() const;
    [[nodiscard]] int getHour() const;
    [[nodiscard]] int getMinute() const;
    [[nodiscard]] int getSecond() const;
    [[nodiscard]] std::string getDow() const;
};

#endif //MY_DATE_H
