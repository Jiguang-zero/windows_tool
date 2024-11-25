//
// Created by 86158 on 2024/11/1.
//

#include <windows.h>
#include <iostream>
#include <winrt/Windows.UI.ViewManagement.h>
#include "../utils/logger/Logger.h"

void changeTheme(const bool isDark) {
    // 打开注册表
    HKEY hKey;
    LOG << "打开注册表";
    if (const auto regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"; RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        const auto systemValueName = L"SystemUsesLightTheme";
        const auto valueName = L"AppsUseLightTheme";
        const DWORD value = isDark ? 1 : 0; // 如果是深色，则将值设置为1（表示浅色），否则为0（表示深色）

        // 设置 AppsUseLightTheme 值
        RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE *>(&value), sizeof(value));

        // 设置 SystemUsesLightTheme 值
        RegSetValueExW(hKey, systemValueName, 0, REG_DWORD, reinterpret_cast<const BYTE *>(&value), sizeof(value));

        // 关闭注册表
        RegCloseKey(hKey);
        LOG << "关闭注册表";

        LOG << "Change Theme Successfully";
    } else {
        LOG << "Change Theme Failed.";
    }
}

inline bool IsColorDark(const winrt::Windows::UI::Color& clr)
{
    return 5 * clr.G + 2 * clr.R + clr.B > 8 * 128;
}

// 如果当前主题是 黑夜模式， 那么返回true
bool themeIsDark() {
    const auto settings = winrt::Windows::UI::ViewManagement::UISettings();
    const auto foreground = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Foreground);

    // Check the current theme (this is just a placeholder; normally you'd read the registry)
    return IsColorDark(foreground);
}

int main(int argc, char** argv) {
    const bool currentThemeIsDark = themeIsDark();
    std::cout << "Current theme is " << (currentThemeIsDark ? "Dark" : "Light") << std::endl;
    const std::string currentTheme = std::string("Current theme: ") + (currentThemeIsDark ? "Dark" : "Light");
    LOG << currentTheme;

    changeTheme(currentThemeIsDark);

    return 0;
}
