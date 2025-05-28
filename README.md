

This is the final project for the Spring 2025 "Practice of programming in cpp and python" course at Peking University, developed by elainafan, wzy240, and XiaoGong1453. 

The project implements a notebook application with embedded Markdown rendering and data encryption.

Thank you for your stars!


## 加密模块说明

CryptoUtils.h和CryptoUtils.cpp是加解密的主要文件，支持各种文件格式(md,jpg,...)的加解密。

qaesencryption.h和qaesencryption.cpp是AES加解密的实现文件，来自第三方库，请勿修改。

mainwindow.h和mainwindow.cpp是加密窗口的一个样例，用于测试和理解接口的功能。

## 源代码说明

### 运行类

`Application` ： 程序运行类，控制整个程序的运行和窗口级别的跳转

### 窗口类

`AppWindow` ： App的主界面类

`DailyPromptWindow` ： 每日推送界面类

`DairyDetailWindow` ： 日记细节展示窗口类（可能以后会删掉）

`Guider` （在 `GuiderWindow` 文件中存放）： 新用户的欢迎界面

`LoginWindow` ： 登录界面类

`MyWidget` ： 实现了隐藏和显示动画的窗口基类，所有窗口类都是这个类的派生

### 控件类

`Calendar` ： 主界面的日历控件类

`DiaryListWidget` ： 主界面的日记本选择窗口类（其中还包含了 `DiaryListEntry` 类，即单个日历条目）

`GradientOverlay` ： 推送界面下部的渐变图层

`SearchWidget` ： 主界面的搜索控件类

`Reminder` ： 主界面的提醒事项控件类

`UserInfoWidge` ： 主界面右上角的用户信息类


### 功能类

`CryptoUtils` ： 加密、解密功能类

`FileOperation` ： 文件管理、读写类（还具有登录管理功能）

`qaesencryption` ： 加密用

`Widget` ： 日记本编辑器类

### 数据结构类

`DiaryList` ：日记本类，存储日记本

`Diary` ： 日记类，存储单篇日记

### 其他文件

`Headers.h` ： 所有头文件

`interfaceRequirement.md` ：需要的所有接口

`Resources.qrc` ： 存储App的所有自带数据（目前主要是我需要用到的一些icons）

`/images` ： （其实更好叫icons，但改起来太麻烦了，算了）所有App用到的icons