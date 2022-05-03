---
layout:     post
title:      数仓工具：基于 docker 搭建 Hadoop 集群
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - 数仓
---

# 1. Windows 环境安装 docker

##### 1.1. 更新 WSL 2 Linux 内核

- 下载  WSL 2 Linux 内核，下载地址：[https://docs.microsoft.com/zh-cn/windows/wsl/wsl2-kernel](https://docs.microsoft.com/zh-cn/windows/wsl/wsl2-kernel)
- 双击下载好的 wsl_update_x64.msi 文件，安装 WSL；

##### 1.2. 安装 docker for windows

- 下载 docker for windows，下载地址：[https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe](https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe)
- 双击 Docker Desktop Installer，开始安装；

##### 1.3. 配置阿里云镜像加速器

- 如果没有阿里云账号，需要注册阿里云开发账号，[https://dev.aliyun.com/](https://dev.aliyun.com/)；

- 进入加速器页面获取镜像加速器地址，[https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors](https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors)

![]({{site.baseurl}}/img-post/docker-hadoop-3.png)

- 打开 Docker Desktop，在桌面右下角找到 docker 鲸鱼小图标，依次点击 **鼠标右键 -> settings -> Docker Engine**，在输入框中输入加速信息；

```
{
  "registry-mirrors": [
    "在此处粘贴上面复制的加速器地址，注意要加引号"
  ],
  "insecure-registries": [],
  "debug": true,
  "experimental": false
}
```

- 如下图所示：
  ![]({{site.baseurl}}/img-post/docker-hadoop-4.png)
- 等待 docker desktop 重启，配置完成。

##### 1.4. WSL 2 installation is incomplete. 错误

![]({{site.baseurl}}/img-post/docker-hadoop-1.png)

- 报错提示使用的wsl2版本老了，需要我们自己手动更新一下，根据提示去微软官网下载最新版的wsl2安装后即可正常打开。

- 下载地址：[https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi)

![]({{site.baseurl}}/img-post/docker-hadoop-1.png)