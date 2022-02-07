---
layout:     post
title:      处理 AndroidKiller 反编译失败问题
subtitle:   报错内容：AndroidKiller 反编译失败，无法继续下一步源码反编译!
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-hacker.jpg
catalog: true
tags:
    - 安卓逆向
---


#1. Mitmproxy 简介

- Mitmproxy本意为中间人攻击代理；
- 在爬虫中主要用作抓取 http_connect、request、response 等数据；
- mitmproxy官方文档地址：https://docs.mitmproxy.org/stable/；
>【提示】
**Mitmproxy 相较于 fillder、wireshark 的不同：**
--  *mitmproxy 不仅可以截获请求、帮助开发者查看、分析、更可以通过自定义脚本进行二次开发，修改爬虫行为*；

#2. 配置 Mitmproxy

### 2.1 安装 mitmproxy
```
brew install mitmproxy
```
安装好后检查安装情况。
```
mitmdump --version
```
得到下图所示内容，表示安装成功。
![查看版本号、支持的Python版本、OpenSSL协议等](https://upload-images.jianshu.io/upload_images/14502986-debcb7f2ee8407ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2.2. 启动 mitmproxy

```
mitmweb    # mitmproxy有三种启动方式，此处使用的命令可以提供一个web交互界面
```
>【提示】
**mitmproxy有三种启动命令：**
(1) mitmweb
-- 提供一个web界面；
-- 代理端口：绑定了 *:8080作为代理端口；
-- 交互界面地址：localhost:8081；
(2) mitmproxy
-- 提供命令行界面；
-- 可以通过命令过滤请求；
(3) mitmdump
-- 【TODO】

得到下图，表示启动成功，端口号默认为 8080。
![mitmproxy服务器绑定了 *:8080作为代理端口](https://upload-images.jianshu.io/upload_images/14502986-af82d9b5d881dc51.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

此时，浏览器自动打开下图页面，这是 mitmproxy 提供的 web 交互界面。
![上图为mitmproxy自动打开的web交互界面](https://upload-images.jianshu.io/upload_images/14502986-e74b7e934c294100.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2.3. 安装CA证书

第一步，将电脑和手机连到同一个 WiFi 中；

第二步，获取本机的内网IP地址；
```
ifconfig
```
得到电脑本机的内网IP地址为 192.168.1.102。
![WeChatd8d30030a2bf45bb1071f0f136698471.png](https://upload-images.jianshu.io/upload_images/14502986-8bbe71fa635b42bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三步，给手机WiFi配置代理
- 服务器地址为电脑内网IP地址，端口为8080
  ![服务器地址为电脑内网IP地址，端口为8080](https://upload-images.jianshu.io/upload_images/14502986-5af48be574233c77.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
  第四步，安装证书

使用手机浏览器访问 mitm.it，得到下图。

![选择自己的手机系统](https://upload-images.jianshu.io/upload_images/14502986-e897339c98d21fc1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我的手机为iPhone，点击 Apple 后得到下图。

![31543663396_.pic.jpg](https://upload-images.jianshu.io/upload_images/14502986-6dffa54561918685.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

点击允许，开始安装。

![](https://upload-images.jianshu.io/upload_images/14502986-c3af2013117d52b7.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

安装完成后，得到已验证的提示。

![61543663399_.pic.jpg](https://upload-images.jianshu.io/upload_images/14502986-609b39af4b793dd1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2.4. 开启证书

手机依次点击：**设置** -> **通用** -> **关于本机** -> **证书信任设置**，开启 mitmproxy 证书。

![微信图片编辑_20200207211352.jpg](https://upload-images.jianshu.io/upload_images/14502986-de294021a948d2c6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> **注意**：
>- 有小伙伴反馈，安卓 7.0 以上版本不再信任证书，需要将证书安装到 root 路径下，这导致数据抓包不成功、手机应用网络不通。
>- 博主还没有做过研究，想提示一下安卓 7.0 用户如果遇到这类问题，可以尝试使用模拟器降低安卓版本，看是否能解决问题，有时间的话麻烦把结果给博主反馈一下，谢谢！

### 2.5 配置完成

此时，mitmweb 页面出现下图内容，红框中的为 mitmproxy 抓取的手机的请求。
![红框中的为 mitmproxy 抓取的手机的请求](https://upload-images.jianshu.io/upload_images/14502986-b4c7033447e8e9a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

到这里，就成功完成了 mitmproxy配置。

# 3. mitmproxy 获取APP数据

### 3。1. 编辑 script.py 文件

- script.py 是用来处理 mitmproxy 获取到的 request 和 response 的 .py 脚本；
- 用户根据业务需求，在该文件中 筛选、处理 request 和 response ；
- 本文模拟的获取 **京东APP** 数据，需要筛选出 url 包含 **jd** 字符的  请求和响应；
```
import json
from urllib.parse import unquote
import re

def response(flow):
	# 提取请求的 url 地址
	request_url = flow.request.url
    # 通过 jd 字符串，过滤出 京东APP 的请求和返回数据
	if bool(re.search(r"jd", request_url)):     
		print("request_url >>> ", request_url)
		response_body = flow.response.text
		response_url = flow.request.url
		print("response_url >>> ", response_url)
		data = json.loads(response_body)
		ware_infos = data.get("wareInfo")
		goods_info = {}
		if ware_infos is not None:
			for ware_info in ware_infos:
				goods_info["wareId"] 	= ware_info.get("wareId")
				goods_info["wname"] 	= ware_info.get("wname")
				goods_info["jdPrice"] 	= ware_info.get("jdPrice")
				goods_info["goodrate"] 	= ware_info.get("good")
				goods_info["reviews"] 	= ware_info.get("reviews")
				goods_info["shopId"] 	= ware_info.get("shopId")
				goods_info["ShopName"] 	= ware_info.get("goodShop").get("goodShopName")
				print(goods_info)

```
### 3.2. 运行 script.py 文件

- 进入 script.py 文件所在文件夹
```
cd /XXX   # 进入 script.py 文件所在文件夹
mitmdump -s script.py
```
![image.png](https://upload-images.jianshu.io/upload_images/14502986-1164973a0b0c2d95.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 3.3. 启动 京东APP，抓取数据

>注意：如果手机尚未配置好，请参考本博另一篇文章：[[爬虫]使用mitmproxy抓包手机APP的配置步骤](https://www.jianshu.com/p/8ee3f9f46d7a)
- 启动 APP
  启动 京东APP 后，命令行窗口得到下图所示返回信息，数据获取成功。
  ![image.png](https://upload-images.jianshu.io/upload_images/14502986-94e82ccdf35e10f0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
