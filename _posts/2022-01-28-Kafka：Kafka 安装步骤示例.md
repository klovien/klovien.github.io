---
layout:     post
title:      Kafka：Kafka 安装步骤示例
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Zookeeper
---

# 1. 准备工作

##### 1.1. 集群规划

  ![]({{site.baseurl}}/img-post/kafka.png)

##### 1.2. 下载

<li>点击此处下载：<a href="https://archive.apache.org/dist/kafka/2.4.1/kafka_2.12-2.4.1.tgz">https://archive.apache.org/dist/kafka/2.4.1/kafka_2.12-2.4.1.tgz</a></li>

##### 1.3. 安装

- 解压缩文件，到常用软件安装位置。

    ```aidl
    tar -zxvf kafka_2.12-2.4.1.tgz
    
    # 重命名为 kafka
    mv kafka_2.12-2.4.1 kafka
    ```

# 2. 修改配置

##### 2.1. 修改配置文件

- 修改 `server.properties`

    ```aidl
    cd /kafka/config
    vi server.properties
    ```

    ```aidl
    # broker的全局唯一编号，不能重复
    broker.id=0
  
    # 删除topic功能使能
    delete.topic.enable=true
  
    # 处理网络请求的线程数量
    num.network.threads=3
  
    # 用来处理磁盘IO的现成数量
    num.io.threads=8
  
    # 发送套接字的缓冲区大小
    socket.send.buffer.bytes=102400
  
    # 接收套接字的缓冲区大小
    socket.receive.buffer.bytes=102400
  
    # 请求套接字的缓冲区大小
    socket.request.max.bytes=104857600
  
    # kafka运行日志存放的路径 
    log.dirs=/usr/local/kafka/kafka-logs
  
    # topic在当前broker上的分区个数
    num.partitions=1
  
    # 用来恢复和清理data下数据的线程数量
    num.recovery.threads.per.data.dir=1
  
    # segment文件保留的最长时间，超时将被删除
    log.retention.hours=168
  
    # 配置连接Zookeeper集群地址
    zookeeper.connect=hadoop102:2181,hadoop103:2181,hadoop104:2181/kafka
    ```
  
##### 2.2. 配置环境变量

- 编辑 `/etc/profile`

  ```aidl
  sudo vi /etc/profile
  
  #KAFKA_HOME
  export KAFKA_HOME=/usr/local/kafka
  export PATH=$PATH:$KAFKA_HOME/bin
  ```

- 启动生效

  ```
  source /etc/profile
  ```

##### 2.3. 分发 kafka

- 分发 kafka

  ```aidl
  xsync kafka/
  ```

- 依次修改 hadoop103、hadoop104 的 `server.properties`
  - hadoop103
    ```aidl
    broker.id=1
    ```
  - hadoop104
    ```aidl
    broker.id=2
    ```
  - 注：broker.id 不得重复。

##### 2.4. 分发 `/etc/profile`

- 分发文件

  ```aidl
  xsync /etc/profile
  ```

- 依次在 hadoop103、hadoop104 启动生效

  ```
  source /etc/profile
  ```


# 3. 运行

##### 3.1. 依次启动

- 注意：
  - **kafka 依赖于 zookeeper，需要先启动 zookeeper**。

- 在 hadoop102、hadoop103、hadoop104 上逐个启动

    ```aidl
    bin/kafka-server-start.sh config/server.properties &
    ```

- 查看运行情况

  ```aidl
  javapsall
  
  
  ==========  hadoop102  =========
  3222 QuorumPeerMain
  2470 DataNode
  3017 NodeManager
  2330 NameNode
  3325 Kafka
  ==========  hadoop103  =========
  1490 Kafka
  1383 QuorumPeerMain
  ==========  hadoop104  =========
  1588 Kafka
  1396 SecondaryNameNode
  1476 QuorumPeerMain
  ```

##### 3.2. 依次关闭

- 逐个关闭
  ```
  bin/kafka-server-stop.sh stop
  
  
  ...
  [2022-05-16 22:50:58,864] INFO [SocketServer brokerId=0] Shutdown completed (kafka.network.SocketServer)
  [2022-05-16 22:50:58,868] INFO [KafkaServer id=0] shut down completed (kafka.server.KafkaServer)
  ```

- 注意：
  - **关闭 kafaka 之后，需要等一会才能关闭，如果关闭 kafka 以后立刻关闭 zookeeper 可能会无法关闭成功**。

##### 3.3. kafka 群启群停

- 进入 `/usr/local/apache-zookeeper-3.5.7/bin` 目录

  ```
  #!/bin/bash

  case $1 in
  "start" ){
    for(( i = 2;i <= 4;i = $i +1));do
      echo ============ hadoop10$i kafka $1 ===================
      ssh hadoop10$i "source /etc/profile;nohup /usr/local/kafka/bin/kafka-server-start.sh /usr/local/kafka/config/server.properties"
    done
  };;
  "stop" ){
    for(( i = 2;i <= 4;i = $i +1));do
      echo ============ hadoop10$i kafka $1 ===================
      ssh hadoop10$i "source /etc/profile;kafka-server-stop.sh"
    done
  };;
  esac
  ```

- 集群启动
  ```
  kafka.sh start
  ```
- 集群关闭
  ```
  kafka.sh stop
  ```


# 4. Kafka 常用命令

#### 4.1. topic 命令

- 命令基本语法及功能描述

  - `bin/zkCli.sh help`，显示所有操作命令


##### 4.2. 消息命令

- 