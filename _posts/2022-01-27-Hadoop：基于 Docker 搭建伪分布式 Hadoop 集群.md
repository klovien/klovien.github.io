---
layout:     post
title:      Hadoop：基于 Docker 搭建伪分布式 Hadoop 集群
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hadoop
---

# 1. Windows 环境安装 docker

#### 1.1. 更新 WSL 2 Linux 内核

- 下载  WSL 2 Linux 内核，下载地址：[https://docs.microsoft.com/zh-cn/windows/wsl/wsl2-kernel](https://docs.microsoft.com/zh-cn/windows/wsl/wsl2-kernel)
- 双击下载好的 wsl_update_x64.msi 文件，安装 WSL；

#### 1.2. 安装 docker for windows 到 D 盘

- 在 `D:\Program Files` 目录下，新建 `Docker` 文件夹；
- 创建 mklink 链接
  ```aidl
  cd "C:\Program Files"
  mklink /j "C:\Program Files\Docker" "D:\Program Files\Docker"
  
  # 得到如下提示，标识创建成功
  C:\Program Files\Docker <<===>> D:\Program Files\Docker 
  ``` 
- 下载 docker for windows，下载地址：[https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe](https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe)
- 双击 Docker Desktop Installer，开始安装；

#### 1.3. 配置阿里云镜像加速器

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

#### 1.4. WSL 2 installation is incomplete. 错误

  ![]({{site.baseurl}}/img-post/docker-hadoop-1.png)

- 报错提示使用的wsl2版本老了，需要我们自己手动更新一下，根据提示去微软官网下载最新版的wsl2安装后即可正常打开。

- 下载地址：[https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi](https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi)

# 2. 准备 CentOS 镜像环境

#### 2.1. 获取一个 CnetOS 镜像

- 直接获取官方镜像即可，此处我是用 CentOS 8，具体步骤不再赘述。

#### 2.2. 替换 mirror 源

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

#### 2.3. 创建 dockerfile

- 创建 dockerfile 文件，注意没有后缀。

  ```aidl
  FROM  pandong423/centos8-alimirror-env:1.0
  
  # 安装必要软件
  RUN yum install -y net-tools which openssh-clients openssh-server iproute.x86_64 wget passwd vim
  
  # 修改root密码    
  RUN echo "root:root" | chpasswd
  
  # 添加root用户到sudoers，允许使用sudo命令    
  RUN echo "root   ALL=(ALL)       ALL" >> /etc/sudoers
  
  # ssh用的公私钥认证，需要把PAM认证中的UsePAM yes改为no    
  RUN sed -i 's/UsePAM yes/UsePAM no/g' /etc/ssh/sshd_config
  
  # 创建ssh所需的文件夹/var/run/sshd,否则ssh启动不起来    
  RUN mkdir /var/run/sshd
  
  # 生成无密码的公私密钥对    
  RUN ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
  
  # 公钥添加授权文件authorized_keys中    
  RUN cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
  
  # 设置初次ssh登录时，无需yes确认登陆    
  RUN echo "StrictHostKeyChecking no" >~/.ssh/config
  RUN echo "alias ll='ls -l'" >> ~/.bashrc \    
  RUN source ~/.bashrc
  
  # 暴露 22 端口
  EXPOSE 22
  CMD ["/usr/sbin/sshd", "-D"]
  ```

#### 2.4.  build 环境镜像

- 创建环境准备镜像
  ```aidl
  docker build -t centos8-hadoop-env:v1.0 . --no-cache
  ```
- 此处我们将新镜像 push 到 dockerhub，以备使用。
  ```aidl
  docker tag centos8-hadoop-env:v1.0 你的dockerhub账号/centos8-hadoop-env:v1.0
  docker push 你的dockerhub账号/centos8-hadoop-env:v1.0
  ```

# 3. 搭建 hadoop 集群

#### 3.1. 创建包含 Hadoop 和 JDK 的镜像

- 创建 dockerfile
  - 刚才已经创建了一个Dockerfile了，先将他移开。

    ```aidl
    mv dockerfile dockerfile.bak
    ```
  - 新建编辑 dockerfile
    - **注意：添加环境变量时可能不生效，如果不生效则使用替换代码，注意修改**。
      ```aidl
      FROM centos8-hadoop-env:v1.0
    
      # 配置 jdk
      ADD jdk-8u212-linux-x64.tar.gz /usr/local/
      RUN mv /usr/local/jdk1.8.0_212 /usr/local/jdk1.8
    
      # 添加环境变量 JAVA_HOME
      ENV JAVA_HOME /usr/local/jdk1.8
      ENV PATH $PATH:$JAVA_HOME/bin
    
      # 注意：上面的添加环境变量代码可能不生效，如果不生效可以尝试用下面的代码替换。
      RUN export JAVA_HOME=/usr/local/jdk1.8
      RUN export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/sbin

      # 配置 hadoop
      ADD hadoop-3.1.3.tar.gz /usr/local
      RUN mv /usr/local/hadoop-3.1.3 /usr/local/hadoop
    
      # 添加环境变量 HADOOP_HOME
      ENV HADOOP_HOME /usr/local/hadoop
      ENV PATH $PATH:$HADOOP_HOME/bin
      
      # 注意：上面的添加环境变量代码可能不生效，如果不生效可以尝试用下面的代码替换。
      RUN export HADOOP_HOME=/usr/local/hadoop
      RUN export PATH=$PATH:$HADOOP_HOME/bin:$HADOOP_HOME/sbin
      ```
- 创建镜像
  ```aidl
  docker build -t centos8-jdk1.8-hadoop3.1.3:v1.0 .
  ```
- 检查端口禁用情况
  - 注意检查哪些端口是被禁用的，自行选择合适的对外暴露端口
    
    ```
    netsh interface ipv4 show excludedportrange protocol=tcp
    ```
    
    我的电脑端口禁用情况如下：  
    
    ```
    协议 tcp 端口排除范围

    开始端口    结束端口
     ----------    --------
          49686       49785
          49786       49885
          49886       49985
          50000       50059     *
          50060       50159
          50160       50259
          50262       50361
          50362       50461
          50462       50561
          50562       50661
          50662       50761
          50762       50861
          50862       50961
          64571       64670
          64671       64770
          64771       64870
          64871       64970

    * - 管理的端口排除。
    ```
    - 我选择 `50260` 做为对外暴露端口。

- 创建 bridge 类型网络，让集群间能网络连通。
  ```aidl
  docker network create --driver bridge hadoop-br
  ```

#### 3.2. 启动 hadoop 容器
- 启动容器
  ```aidl
  docker run -itd --network hadoop-br --name hadoop1 -p 50260:50070 -p 8088:8088 centos8-jdk1.8-hadoop3.1.3:v1.0
  docker run -itd --network hadoop-br --name hadoop2 centos8-jdk1.8-hadoop3.1.3:v1.0
  docker run -itd --network hadoop-br --name hadoop3 centos8-jdk1.8-hadoop3.1.3:v1.0
  ```

- 检查网络
  ```aidl
  [
    {
        "Name": "hadoop-br",
        "Id": "1ccc41f2821cd700882eca4c161f2a245c9a17c365fc808ce1ddfd0f088fb274",
        "Created": "2022-05-04T10:53:24.57192Z",
        "Scope": "local",
        "Driver": "bridge",
        "EnableIPv6": false,
        "IPAM": {
            "Driver": "default",
            "Options": {},
            "Config": [
                {
                    "Subnet": "172.18.0.0/16",
                    "Gateway": "172.18.0.1"
                }
            ]
        },
        "Internal": false,
        "Attachable": false,
        "Ingress": false,
        "ConfigFrom": {
            "Network": ""
        },
        "ConfigOnly": false,
        "Containers": {
            "08a2bb09245b86d3cbf708987740f6f65edbdae8576f2fcceca5abcdcdaa2d97": {
                "Name": "hadoop3",
                "EndpointID": "114f83d2fb2181d9ac446c0ac2183440c16da46c6446b292fca2825373acc0b6",
                "MacAddress": "02:42:ac:12:00:04",
                "IPv4Address": "172.18.0.4/16",
                "IPv6Address": ""
            },
            "b38a377496a3cd83bf8965365913cd83da8319cff42ca4e0cc8d4318a3306ceb": {
                "Name": "hadoop1",
                "EndpointID": "88a0042804a9848451928f608d33cb16300364aba84a7b108b02727882b23e69",
                "MacAddress": "02:42:ac:12:00:02",
                "IPv4Address": "172.18.0.2/16",
                "IPv6Address": ""
            },
            "f417a6f26827993d83c0d17c3a69365a6bd832d5dbd4a306da495c3f3d968532": {
                "Name": "hadoop2",
                "EndpointID": "6e042fac8decedf98cc8d6ca9bb7c0ddb193e13a8eb8873d445c9105373c34aa",
                "MacAddress": "02:42:ac:12:00:03",
                "IPv4Address": "172.18.0.3/16",
                "IPv6Address": ""
            }
        },
        "Options": {},
        "Labels": {}
    }
  ]
  ```
  - 查看得知三台机器的 IP 分别为：
    ```aidl
    hadoop1   172.18.0.2/16
    hadoop2   172.18.0.3/16
    hadoop3   172.18.0.4/16
    ```
    
- 检查是否能互相 ping 通、是否能 ssh 免密登录
  - hadoop1
    ```aidl
    docker exec -it hadoop1 bash
    
    ping hadoop2
    ping hadoop3
    
    ssh hadoop2
    ```
  - hadoop2
    - 略
  - hadoop3
    - 略

#### 3.3. 安装配置 Hadoop

- 进入 $Hadoop
  ```aidl
  cd $HADOOP_HOME/etc/hadoop/
  ```
- 编辑 core-site.xml
  ```aidl
  <property>
    <name>fs.defaultFS</name>
    <value>hdfs://hadoop1:9000</value>
  </property>
  <property>
    <name>hadoop.tmp.dir</name>
    <value>file:/home/hadoop/tmp</value>
  </property>
  <property>
    <name>io.file.buffer.size</name>
    <value>131702</value>
  </property>
  ```
  
- 编辑 hdfs-site.xml
  ```aidl
  <property>
     <name>dfs.namenode.name.dir</name>
     <value>file:/home/hadoop/hdfs_name</value>
   </property>
   <property>
     <name>dfs.datanode.data.dir</name>
     <value>file:/home/hadoop/hdfs_data</value>
   </property>
   <property>
     <name>dfs.replication</name>
     <value>2</value>
   </property>
   <property>
     <name>dfs.namenode.secondary.http-address</name>
     <value>hadoop1:9001</value>
   </property>
   <property>
     <name>dfs.webhdfs.enabled</name>
     <value>true</value>
   </property>
  ```
  
- 编辑 mapred-site.xml
  ```aidl
  <property>
     <name>mapreduce.framework.name</name>
     <value>yarn</value>
  </property>
  <property>
   <name>mapreduce.jobhistory.address</name>
   <value>hadoop1:10020</value>
  </property>
  <property>
   <name>mapreduce.jobhistory.webapp.address</name>
   <value>hadoop1:19888</value>
  </property>
  ```

- 编辑 yarn-site.xml
  ```aidl
  <property>
     <name>yarn.nodemanager.aux-services</name>
     <value>mapreduce_shuffle</value>
  </property>
  <property>
   <name>yarn.nodemanager.auxservices.mapreduce.shuffle.class</name>
   <value>org.apache.hadoop.mapred.ShuffleHandler</value>
  </property>
  <property>
   <name>yarn.resourcemanager.address</name>
   <value>hadoop1:8032</value>
  </property>
  <property>
   <name>yarn.resourcemanager.scheduler.address</name>
   <value>hadoop1:8030</value>
  </property>
  <property>
   <name>yarn.resourcemanager.resource-tracker.address</name>
   <value>hadoop1:8031</value>
  </property>
  <property>
   <name>yarn.resourcemanager.admin.address</name>
   <value>hadoop1:8033</value>
  </property>
  <property>
   <name>yarn.resourcemanager.webapp.address</name>
   <value>hadoop1:8088</value>
  </property>
  ```







