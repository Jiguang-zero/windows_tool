//
// Created by 86158 on 2024/11/1.
//

#include <windows.h>
#include <iostream>
#include <winrt/Windows.UI.ViewManagement.h>
#include "../utils/logger/Logger.h"

void changeTheme(const bool isDark) {
    // ��ע���
    HKEY hKey;
    LOG << "��ע���" << utils::endl;
    if (const auto regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"; RegOpenKeyExW(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        const auto systemValueName = L"SystemUsesLightTheme";
        const auto valueName = L"AppsUseLightTheme";
        const DWORD value = isDark ? 1 : 0; // �������ɫ����ֵ����Ϊ1����ʾǳɫ��������Ϊ0����ʾ��ɫ��

        // ���� AppsUseLightTheme ֵ
        RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<const BYTE *>(&value), sizeof(value));

        // ���� SystemUsesLightTheme ֵ
        RegSetValueExW(hKey, systemValueName, 0, REG_DWORD, reinterpret_cast<const BYTE *>(&value), sizeof(value));

        // �ر�ע���
        RegCloseKey(hKey);
        LOG_LINE("�ر�ע���");

        LOG_LINE("Change Theme Successfully");
    } else {
        LOG_LINE("Change Theme Failed.");
    }
}

inline bool IsColorDark(const winrt::Windows::UI::Color& clr)
{
    return 5 * clr.G + 2 * clr.R + clr.B > 8 * 128;
}

// �����ǰ������ ��ҹģʽ�� ��ô����true
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
    LOG_LINE(currentTheme);

    changeTheme(currentThemeIsDark);

    return 0;
}
