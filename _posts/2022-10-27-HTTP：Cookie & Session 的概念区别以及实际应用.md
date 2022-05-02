---
layout:     post
title:      HTTP：Cookie & Session 的概念区别以及实际应用
subtitle:   
date:       2022-02-07
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: false
tags:
    - HTTP
---


#1.前言

- 在爬虫面试过程中，Cookie 和 Session 相关的知识点是经常遇到的问题之一；
- Cookie & Session 配合 Http 协议，是最基础、最广泛的用户识别认证实现方式。

# 2. Cookie

##### 2.1. Cookie 的基本概念

- Cookie 是一段键值对格式的文本信息；
- 用户通过 Http 协议访问服务器的时候，服务器会将 Cookie 返回给客户端浏览器，并将这些数据加上一些限制条件，例如有效期（Expire Time）；
- 当用户下次访问的时候，Cookie 又被完整的带给服务器，服务器凭借 Cookie 即可辨认用户的状态；

### 2.2. Cookie 的优缺点

- 优点：数据存储在客户端，减轻服务器的压力，提高网站性能；
- 缺点：安全性不高，在客户端很容易被查看，也有可能会被截获并破解；

# 3. Session 基本概念

##### 3.1. Session 的基本概念

- Session 又称会话控制，是一种网络通讯会话机制；
- 用户通过 Http 协议访问服务器的时候，服务器会在内存区域保存一个 Session  对象，并生成 session id；
- 服务器将 session id 返回给客户端，客户端将其保存在 Cookie 中；
- 当用户下次访问的时候，服务器凭借访问请求所带 Cookie 中的 session id，即可找到对应的 Session 验证用户身份，并服务于访问请求处理；

##### 3.2. Session 的优缺点

- 优点：安全性较高，登陆验证后账号密码等信息均保存于服务器，降低了暴露的几率；
- 缺点：占用服务器资源，如果并发访问过多，Session 大量积聚会导致内存溢出；

# 4. Cookie & Session 的区别

##### 4.1.  存取方式：

- Cookie 中只能保存 ASCII 字符串，如果需要存取 Unicode 或者二进制数据，需要先进行编码；
- Session 中能存取任何类型的数据，包括 str、int、list、map等；

##### 4.2. 隐私策略

- Cookie 存储在客户端，对用户是可见的，客户端上的一些程序可能会窥探、复制、修正 Cookie 中的内容；
- Session 存储于服务器上，对客户端是透明的，不存在敏感信息泄露风险；

##### 4.3. 有效期

- Cookie 可以是设置一个数字很大的过期时间属性，以保证 Cookie 长期有效；
- Seesion 只要浏览器关闭就会失效，且如果 Session 设置的时间超长，服务器积累的 Seesion 越来越多会导致内存溢出；

##### 4.4.服务器压力不同

- Cookie 保存在客户端，不占用服务器资源，如果并发访问的用户比较多，Cookie 是最优选择；
- Seesion 是保管在服务器端的，每个用户都会产生一个 Session，如果并发访问 的用户过多，会耗费大量的服务器资源；

##### 4.5.浏览器支持

- Cookie 需要浏览器支持，如果浏览器禁用或不支持 Cookie，则会话跟踪就会失效；
- 如果浏览器禁用 Cookie，则需要 Session + url 重写，即在请求 url 后面加上 session id；

##### 4.6. 跨域支持

- Cookie 支持跨域名访问；
- Seesion 只在域名内有效；

# 5. Cookie & Session 的联系

- Session 需要基于 Cookie 实现，即服务器生成 session 之后将 session id 发送给客户端，客户端下次访问的时候通过 session id 找到对应的 Session ，验证用户身份并服务于访问请求处理；
- 如果浏览器禁用 Cookie，则需要 Session + url 重写，即在请求 url 后面加上 session id；

# 6. 实际应用

##### 6.1. Cookie

- 登录 Github 账号的时候，Github 服务器会在登录成功后返回 Cookie 给客户端，当用户下次请求的时候就不用再进行登录操作；

##### 6.2. Session

- 用户登录购物商城的时候，服务器会生成 Session 对象，并分配 session id 给用户，之后用户在整个商城添加商品到购物车的时候，添加商品的行为都会与 session id 关联起来，用户就可以在购物车上看到自己添加的所有商品。
