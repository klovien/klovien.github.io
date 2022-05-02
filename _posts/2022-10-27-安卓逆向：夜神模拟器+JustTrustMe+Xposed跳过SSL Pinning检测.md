---
layout:     post
title:      安卓逆向：夜神模拟器+JustTrustMe+Xposed跳过SSL Pinning检测
subtitle:   
date:       2022-02-07
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - 安卓逆向
---

# Android SSL pinning

##### SSL 协议

SSL（安全套接字层）又被称为TLS（数据层安全协议），是一种为网络通信提供数据完整性的一种安全协议。它位于TCP/IP协议与各种应用协议之间。SSL协议主要分为两个部分：Handshake Protocol和Record Protocol。

Handshake protocol是用来协商加密通信数据的密钥，Record Protocol定义传输内容的格式。想了解handshake Protocol以及Record protocol的具体细节可以自己去查阅资料，这里就不详细介绍了。

在SSL/TLS通信中，客户端通过数字证书判断服务器是否可信，并采用证书的公钥与服务器进行加密通信。然而，在开发者在代码中不检查服务器证书的有效性，或选择接受所有的证书时，这种做法可能导致的问题是中间人攻击。

##### Android SSL Certificate Pinning

SSL Pinning技术指的是在应用程序中只信任固定证书或是公钥。应用程序开发人员在程序中使用SSL pinning技术作为应用流量的附加安全层。

通常我们在测试一个应用的时候会设置代理，从而获取应用在运行过程中的流量。我们使用代理工具获取流量的话，需要在设备中安装我们自己的证书到可信任的根证书中，这样应用程序才会将我们的证书视为可信任的有效证书，允许代理工具拦截应用的流量。

但是使用的SSL pinning技术的应用程序，只信任指定的证书，那么我们就算把我们的证书安装到设备中，应用程序也不会信任我们的证书，这样的话我们就不能通过代理的方式来拦截应用的流量了。

想要绕过SSL pinning的话，只需要将证书校验的过程绕过就可以。那么我们可以通过hook的方式来修改证书校验过程，这样就能成功的绕过SSL pinning了。比较常见的hook工具有两种，分别是frida和Xposed，这两种工具中都有hook所有https证书校验方法的模块。

# 安装模拟器 + Xposed + JustTrustMe

此处使用早期的夜神模拟器，[点此下载 nox_setup_v5.1.0.0_android_4.4.2](https://pan.baidu.com/s/1LWnsnvH8h41Jp9KHuIP7rg?pwd=mv00)。

##### 安装 Xposed 并更新

[点击此处下载 de.robv.android.xposed.installer_v33_36570c.apk](https://pan.baidu.com/s/1fsPidu_IjP-fS1d0OZP7iw?pwd=v7tm)，下载成功够直接拖进模拟器安装。

下载后直接模拟器安装，安装成功后依次打开 `Xposed Installer > 框架 > 安装/更新`，之后 `重启模拟器`，重新打开 `Xposed Installer`，此时发现 `激活` 下方连个数字已经变 `绿色`，表示安装成功。

![安装成功后的界面]({{site.baseurl}}/img-post/xposed-installler.png)

>注意：在安装好 Xposed Installer 以后，就要完成更新，否则安装 JustTrustMe 会失败！

##### 安装 JustTrustMe 

[点击此处下载 JustTrustMe.apk](https://pan.baidu.com/s/1lB7ON43SHXUQ-3yTS1mq7g?pwd=kin2)，下载成功后直接拖进模拟器进行安装。

安装好以后启动 Xposed点击 `模块`，进入以后勾选JustTrustMe尾部的复选框，即完成了配置。

![配置成功后的界面]({{site.baseurl}}/img-post/just-trus-me.png)

