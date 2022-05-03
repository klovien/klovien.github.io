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

# 2. CentOS 镜像环境准备

##### 2.1. 获取一个 CnetOS 镜像

- 直接获取官方镜像即可，此处我是用 CentOS 8，具体步骤不再赘述。

##### 2.2. 替换 mirror 源

- 由于 CentOS8 于 2021年12月31日 停止了源的服务，所以需要手动替换 mirror 源，否则 yum 安装的时候会报错。
  - 报错内容：
    ```
    #5 3.638 Error: Failed to download metadata for repo 'AppStream': Cannot prepare internal mirrorlist: No URLs in mirrorlist
    ```
    
- 启动容器
- 在容器内部，我们进行替换源操作。

    ```aidl
    cd /etc/yum.repos.d/
    sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*
    sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*
    yum update -y
    wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-vault-8.5.2111.repo
    yum clean all
    yum makecache
    ```
  
- 到这一步时仍然会报错，报错如下：

  ```aidl
  Errors during downloading metadata for repository 'base':
    - Status code: 404 for https://mirrors.aliyun.com/centos/8/BaseOS/x86_64/os/repodata/repomd.xml (IP: 202.100.222.210)
  ```
  
- 这是因为阿里云改了源文件结构，但是在repo配置文件中没有同步修改，需要修改一下repo文件中的路径信息。

  ```aidl
  sed -i 's/$releasever/8-stream/g' /etc/yum.repos.d/CentOS-AppStream.repo
  sed -i 's/$releasever/8-stream/g' /etc/yum.repos.d/CentOS-Base.repo
  sed -i 's/$releasever/8-stream/g' /etc/yum.repos.d/CentOS-Extras.repo
  ```
- 重新执行 `yum makecache`，提示 `Metadata cache created.`。
- docker commit 打包容器成新的镜像。
  ```docker commit d32673714e72 centos8-alimirror-env:1.0```
- 此时我们可以 push 一下这个镜像，以方便其他项目使用。
  ```aidl
  docker login -u 你的 dockerhub 账号
  
  docker tag centos8-alimirror-env:1.0 你的dockerhub账号/centos8-alimirror-env:1.0
  docker push
  ```
  - 注意：
    - push 前一定要给上传的镜像重新打一个 tag，否则 push 的时候会报错：`denied: requested access to the resource is denied`。

##### 2.3. 创建 dockerfile

```aidl
FROM  centos8-alimirror-env:1.0

# 安装必要软件
RUN yum install -y net-tools which openssh-clients openssh-server iproute.x86_64 wget passwd vim \    

# 修改root密码    
&& echo "root:root" | chpasswd \    

# 添加root用户到sudoers，允许使用sudo命令    
&& echo "root   ALL=(ALL)       ALL" >> /etc/sudoers \    

# ssh用的公私钥认证，需要把PAM认证中的UsePAM yes改为no    
&& sed -i 's/UsePAM yes/UsePAM no/g' /etc/ssh/sshd_config \    

# 创建ssh所需的文件夹/var/run/sshd,否则ssh启动不起来    
&& mkdir /var/run/sshd \    

# 生成无密码的公私密钥对    
&& ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa \    

# 公钥添加授权文件authorized_keys中    
&& cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys \    

# 设置初次ssh登录时，无需yes确认登陆    

&& echo "StrictHostKeyChecking no" >~/.ssh/config \    
&& echo "alias ll='ls -l'" >> ~/.bashrc \    
&& source ~/.bashrcEXPOSE 22CMD ["/usr/sbin/sshd", "-D"]
```






