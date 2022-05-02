---
layout:     post
title:      HTTP：http 与 https 的概念区别以及实际应用
subtitle:   
date:       2022-02-07
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: false
tags:
    - HTTP
---


# 1. 前言
在爬虫岗位面试过程中，经常会被问到一个问题，就是 http/https 协议的工作原理，以及 http 和 https 的区别。
作为爬虫工程师需要具备的基本知识，对于 Http/Https 的概念以及区别的理解是爬虫工程师必须要掌握的。

# 2. Http & Https 协议基本概念
##### 2.1. Http 协议
- Http 协议是 Hyper Text Transfer Protocal （超文本传输协议）的缩写，适用于从万维网服务器传输超文本协议到本地浏览器的传输协议；
  
    ![]({{site.baseurl}}/img-post/http-1.png)
  
##### 2.2. Https 协议
- Http 协议是 Hyper Text Transfer Protocal over Secure Socket Layer（基于安全层套接的超文本传输协议）的缩写，是在 Http 基础上加了 SSL（Secure Socket Layer）层；
- SSL 主要用于 web 的安全传输协议，在传输层对网络连接进行加密，保障在网络上的数据传输安全；
- CA（Certificate Authority）证书被应用于 Https 加密操作中，该证书由证书认证机构颁布，里面包含了公钥；
- Https 使用的加密技术：
>-- **对称密钥加密**：
>概念：发送端采用已知的算法对信息进行加密，比如 MD5 或者 Base64，之后秘钥被加密传输给接收端，接收端使用秘钥对接收的信息进行解密；
>缺点：一旦被窃听，或者信息被劫持，秘钥皆可能被破解；
>
>-- **非对称密钥加密**：
>概念：服务器告诉客户端，按照给定的公开密钥进行加密，服务器接收到秘钥后，再按照自己的秘钥进行解密；
>缺点：可能有人模仿服务器向客户端发送公共秘钥，而且非对称加密的效率比较低；
>
>-- **证书加密**：
>服务器向数字证书认证机构提出公开密钥申请，证书机构识别申请宁者的身份后将公开密钥做数字签名，并将公开密钥放入证书，之后服务器将证书发送给客户端，客户端通过证书中的数字签名验证证书的真伪，以确保公钥为真。
- 现在，越来越多的网站支持 Http 而不是 Https，不过仍然有部分站点使用 Http 协议。
> 2017年1月发布的Chrome 56浏览器，开始把收集密码或信用卡数据的HTTP页面标记为“不安全”，若用户使用2017年10月推出的Chrome 62，带有输入数据的HTTP页面和所有以无痕模式浏览的HTTP页面都会被标记为“不安全”。此外，苹果公司强制所有iOS App在2017年1月1日前使用HTTPS加密。
>如下图：

![]({{site.baseurl}}/img-post/http-2.png)

# 3. Http 与 Https 协议的区别
- Http 是明文传输，Https 使用 SSL 进行了加密；
- Http 是无状态的连接，Https 使用了信息加密和安全认证；
- Http 端口号为 80， Https 端口号为 443；
- ***使用 Http 协议时数据获取的会更齐全，而使用 Https 获取数据则会有缺失***。
# 4. 实践中爬取 Http 与 Https 的区别
- 在爬虫开发中，确定了 url 地址以后，尽可能采用 Http，以获取更多数据，而不是采用 Https 协议获取。
- 不过现在很多站点都不支持 Http 了，所以在开发时需要做好测试工作，确定网页是否同时支持 Http 和 Https。
