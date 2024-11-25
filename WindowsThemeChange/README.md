# Windows Theme Change
Windows 系统更改主题

由于之前手动点击菜单“个性化”更改主题，总会连同桌面壁纸一块更改，及其不方便。

并且点击不方便（对于个人而言）

因此，写了这个简单的程序（`Windows11`平台测试过，其他没有）

> **该应用只是单独将系统主题更改而已，建议其他应用比如Google等打开主题随系统变化的功能**

# 编译
CMake 项目，msvc编译

或者可以直接到`release`查找编译完的二进制文件

# 使用说明
可以直接打开编译完的程序

## 额外方案
另外，这里提供一种使用方案，方便化

将编译完的文件存储到任何位置，比如 `D:\computer\tools\system\WindowsChangeTheme\WindowsThemeChange.exe`

然后在另外常用的文件夹目录下，新建快捷方式
比如 `D:\computer\tool\quickOpen`
在此目录下，新建`WindowsThemeChange.exe`文件的快捷方式，快捷方式名字可以为`changeTheme`

然后打开环境变量，将`D:\computer\tool\quickOpen`添加到 用户或者系统的`path`中

然后如果需要快速更改主题，那么打开 运行界面 (`Win + R`)，输入 `changeTheme`（大小写无关）即可

