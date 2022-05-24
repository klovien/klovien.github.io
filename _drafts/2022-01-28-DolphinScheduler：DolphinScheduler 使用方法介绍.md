---
layout:     post
title:      DolphinScheduler：DolphinScheduler 使用方法介绍
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Zookeeper
---

# 1. 准备工作

#### 1.1. 集群规划

  ![]({{site.baseurl}}/img-post/flume-1.png)

  

#### 1.2. 下载

<li>点击此处下载：<a href="https://dlcdn.apache.org/flume/1.9.0/apache-flume-1.9.0-bin.tar.gz">https://dlcdn.apache.org/flume/1.9.0/apache-flume-1.9.0-bin.tar.gz</a></li>

#### 1.3. 安装

- 解压缩文件，到常用软件安装位置。

    ```aidl
    tar tar -zxf apache-flume-1.9.0-bin.tar.gz
  
    # 移动文件到指定位置
    mv apache-flume-1.9.0-bin/ flume
    ```

# 2. 修改配置

#### 2.1. 删除低版本 guava 包

- 在 flume 文件夹 `/lib` 目录下执行

    ```aidl
    cd /lib
    mv guava-11.0.2.jar guava-11.0.2.jar_bak
    ```

#### 2.2. 修改日志配置

- 在 flume 文件夹 `/conf` 目录下修改 `log4j.properties`

    ```aidl
    # 指定日志文件的绝对路径
    flume.log.dir=/usr/local/flume/logs
    ```

#### 2.3. 调整堆内存

- 在 `flume/conf/` 目录下编辑 `flume-env.sh`

  ```
  cd flume/conf/
  mv flume-env.sh.template flume-env.sh
  
  vim flume-env.sh
  ```

- 修改内存：
  - 最小 1G、最大 4G；
  - 根据实际需要，尽可能多配一点防止挂掉；

  ```
  export JAVA_OPTS="-Xms1000m -Xmx4000m -Dcom.sun.management.jmxremote"
  ```

  - Xms，表示 JVM Heap（堆内存）最小尺寸，初始分配；
  - Xmx，表示 JVM Heap（堆内存）最大允许的尺寸，按需进行分配。

#### 2.4. 分发文件

- xsync 分发文件

  ```aidl
  xsync flume/
  ```

