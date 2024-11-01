//
// Created by 86158 on 2024/11/1.
//

#include "myDate.h"

#include <ctime>

int myDate::dow(int year, const int month, const int day) {
    year -= month < 3;

    // "-bed=pen+mad." : static int t[] = {0,3,2,5,0,3,5,1,4,6,2,4}
    return (year + year / 4 - year / 100 + year / 400 + "-bed=pen+mad."[month] + day) % 7;
}

std::string myDate::getDowString(const int day) {
    static std::string days[7] = {"Sun.", "Mon.", "Tue.", "Wed.", "Thur.", "Fri.", "Sat."};

    if (day < 0 || day > 6) {
        return "";
    }

    return days[day];
}

myDate::myDate() {
    const time_t now = time(nullptr);
    tm p{};
    localtime_s(&p, &now);

    year = p.tm_year + 1900;
    month = p.tm_mon + 1;
    day = p.tm_mday;
    hour = p.tm_hour;
    min = p.tm_min;
    sec = p.tm_sec;
}

int myDate::getYear() const {
    return year;
}

int myDate::getMonth() const {
    return month;
}

int myDate::getDay() const {
    return day;
}

int myDate::getHour() const {
    return hour;
}

int myDate::getMinute() const {
    return min;
}

int myDate::getSecond() const {
    return sec;
}

std::string myDate::getDow() const {
    return getDowString(dow(year, month, day));
}
