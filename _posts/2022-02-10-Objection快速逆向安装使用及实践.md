---
layout:     post
title:      Objection快速逆向安装使用及实践.md
subtitle:   过程全记录
date:       2022-02-10
author:     dex0423
header-img: img/post-bg-hacker.jpg
catalog: true
tags:
    - 安卓逆向
---


#### 1. Objection 简介

Objection 是一款集成的逆向分析工具，它将逆向分析和内存漫游的常用工具进行了整合，使用它可以很方便的进行逆向分析，支持 Android 和 iOS 两大平台，

Objection 提供以下功能：
- 内存搜索；
- 类搜索;
- 模块搜索；
- 方法 Hook；
- 打印参数；
- 返回值；
- 调用栈 

#### Objection 三大组成部分：
- Objection 安装


#### Objection 安装

```
pip install -U objection

```

#### 3. adb 工具

将 `Android\Sdk\platform-tools` 路径添加到环境变量，之后打开 cmd 命令行工具，输入 `adb devices` 即可查看连接的设备。
```
C:\Users\pando>adb devices
List of devices attached
emulator-5554   device
```

#### 4. JAVA 安装

 - JAVA 安装步骤自行百度，安装完以后 cmd 输入 `java -version` 得到下面的提示，表示安装成功。
```
C:\Users\pando>java -version
java version "17.0.2" 2022-01-18 LTS
Java(TM) SE Runtime Environment (build 17.0.2+8-LTS-86)
Java HotSpot(TM) 64-Bit Server VM (build 17.0.2+8-LTS-86, mixed mode, sharing)
```

#### 5. 反编译工具 apktool

- 访问[https://ibotpeaches.github.io/Apktool/](https://ibotpeaches.github.io/Apktool/)，下载 apktool.jar；
- 访问 [https://raw.githubusercontent.com/iBotPeaches/Apktool/master/scripts/windows/apktool.bat](https://raw.githubusercontent.com/iBotPeaches/Apktool/master/scripts/windows/apktool.bat)，得到的文本另存为 `apktool.bat` 文件，和 apktool.jar 保存到同一个目录中；
- 添加文件夹路径到系统环境变量 PATH 中，apktool 即可生效。

#### 6. JADX

- 访问本站[https://dex0423.github.io/resources/](https://dex0423.github.io/resources/)，即可下载。
- 也可访问[https://github.com/skylot/jadx/tree/v1.0.0](https://github.com/skylot/jadx/tree/v1.0.0)下载最新版。

#### 7. JEB

- 访问本站[https://dex0423.github.io/resources/](https://dex0423.github.io/resources/)，即可下载（注意参考提示的安装以及使用方法）。


