---
layout:     post
title:      Zookeeper：Zookeeper安装步骤示例
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

  ![]({{site.baseurl}}/img-post/zookeeper.png)

#### 1.2. 下载

<li>点击此处下载：<a href="https://yuyan.fandom.com/zh/wiki/%E4%B8%89%E5%8F%B6%E8%8D%89%E5%9B%BD%E9%99%85%E8%AF%AD">apache-zookeeper-3.5.7-bin.tar.gz</a></li>

#### 1.3. 安装

- 解压缩文件，到常用软件安装位置。

    ```aidl
    tar -zxvf apache-zookeeper-3.5.7-bin.tar.gz
    # 移动文件到指定位置
    cp -r ./apache-zookeeper-3.5.7-bin/* /指定目录/zookeeper-3.5.7 & rm -rf apache-zookeeper-3.5.7-bin
    ```

# 2. 修改配置

#### 2.1. 配置 dataDir 和 myid

- 创建 `zkData` 目录

    ```aidl
    cd /指定目录/zookeeper-3.5.7
    mkdir zkData
    cd zkData
    ```

- 修改 myid 为 1

    ```aidl
    vim myid
    ```
    ```aidl
    1
    ```
- 保存退出

- 编辑 cfg 文件

    ```aidl
    cd ../conf
    mv zoo_sample.cfg zoo.cfg
    vim zoo.cfg 
    ```

- 修改以下内容：

    ```aidl
    dataDir=/指定目录/zookeeper-3.5.7/zkData
    ```
    
    ```aidl
    #######################cluster##########################
    
    server.1=hadoop102:2888:3888
    
    server.2=hadoop103:2888:3888
    
    server.3=hadoop104:2888:3888
    ```

- 代码含义：
  - `server.A=B:C:D`
    - A，第几号服务器；
    - B，服务器 ip 地址；
    - C，服务器与集群 Leader 交换信息的端口；
    - D，执行选举时服务器相互通信的端口。`


#### 2.2. 分发到 hadoop103 和 hadoop104

- xsync 分发

    ```aidl
    sudo xsync zookeeper-3.5.7/ 
    ```

#### 2.3. 修改 hadoop103 和 hadoop104 的设置

- hadoop103

    ```aidl
    # 链接 hadoop103
    ssh hadoop103
    
    # 获取 root 权限
    su
    
    # 编辑 myid
    vim /指定目录/zookeeper-3.5.7/zkData/myid
    
    2
    
    # 保存退出
    ```
- hadoop104

    ```aidl
    # 链接 hadoop104
    ssh hadoop104
    
    # 获取 root 权限
    su
    
    # 编辑 myid
    vim /指定目录/zookeeper-3.5.7/zkData/myid
    
    3
    
    # 保存退出
    ```
# 3. 运行

#### 3.1. 启动

- 在 hadoop102、hadoop103、hadoop104 上逐个启动

    ```aidl
    cd /zookeeper-3.5.7/bin
    ./zkServer.sh start
    
    # 启动成功
    ZooKeeper JMX enabled by default
    Using config: /usr/local/apache-zookeeper-3.5.7/bin/../conf/zoo.cfg
    Starting zookeeper ... STARTED
    ```

#### 3.2. 关闭

- 逐个关闭
  ```
  cd /bin
  ./zkServer.sh    stop
  
  # 关闭成功
  ZooKeeper JMX enabled by default
  Using config: /usr/local/apache-zookeeper-3.5.7/bin/../conf/zoo.cfg
  Stopping zookeeper ... STOPPED
  ```

#### 3.3. Zookeeper 群启群停

- 进入 `/usr/local/apache-zookeeper-3.5.7/bin` 目录
    ```
    cd /usr/local/apache-zookeeper-3.5.7/bin
    ```
- 编辑 zkEnv.sh
    ```
    vim zkEnv.sh
    ```

    找到下面这段代码：

    ```
    if [[ -n "$JAVA_HOME" ]] && [[ -x "$JAVA_HOME/bin/java" ]];  then
    JAVA="$JAVA_HOME/bin/java"
    elif type -p java; then
    JAVA=java
    else
    echo "Error: JAVA_HOME is not set and java could not be found in PATH." 1>&2
    exit 1
    fi
    ```
  
    在上面这段代码前，添加 `JAVA_HOME`

    ```aidl
    JAVA_HOME="/usr/local/bin/jdk1.8"
    ```
- 分发 `zkEnv.sh`
    ```
    xsync ./zkEnv.sh
    ```
- 编辑 `zk.sh`

    ```
    vim zk.sh
    ```

    ```aidl
    #!/bin/bash
    
    case $1 in
    "start"){
        for i in hadoop102 hadoop103 hadoop104
        do
            ssh $i "/usr/local/apache-zookeeper-3.5.7/bin/zkServer.sh start"
        done
    };;
    "stop"){
        for i in hadoop102 hadoop103 hadoop104
        do
            ssh $i "/usr/local/apache-zookeeper-3.5.7/bin/zkServer.sh stop"
        done
    };;
    "status"){
        for i in hadoop102 hadoop103 hadoop104
        do
            ssh $i "/usr/local/apache-zookeeper-3.5.7/bin/zkServer.sh status"
        done
    };;
    esac
    ```

    ```aidl
    # 修改权限
    chmod 777 zk.sh
    ```
- 集群启动 Zookeeper

    ```
    ./zk.sh start
  
  
    ZooKeeper JMX enabled by default
    Using config: /usr/local/apache-zookeeper-3.5.7/bin/../conf/zoo.cfg
    Starting zookeeper ... STARTED
    ZooKeeper JMX enabled by default
    Using config: /usr/local/apache-zookeeper-3.5.7/bin/../conf/zoo.cfg
    Starting zookeeper ... already running as process 7970.
    ZooKeeper JMX enabled by default
    Using config: /usr/local/apache-zookeeper-3.5.7/bin/../conf/zoo.cfg
    Starting zookeeper ... STARTED
    ```

# 4. Zookeeper 常用命令

- 命令基本语法及功能描述

  - `bin/zkCli.sh help`，显示所有操作命令

  - `bin/zkCli.sh ls path [watch]`，使用 ls 命令来查看当前znode中所包含的内容

  - `bin/zkCli.sh ls2 path [watch]`，查看当前节点数据并能看到更新次数等数据

  - `bin/zkCli.sh create`，普通创建

  - `bin/zkCli.sh -s`，含有序列

  - `bin/zkCli.sh -e`，临时（重启或者超时消失）

  - `bin/zkCli.sh get path [watch]`，获得节点的值

  - `bin/zkCli.sh set`，设置节点的具体值

  - `bin/zkCli.sh stat`，查看节点状态

  - `bin/zkCli.sh delete`，删除节点

  - `bin/zkCli.sh rmr`，递归删除节点

# 5. 常见错误

#### 5.1. 启动 zookeeper 时报错：`Error contacting service. It is probably not running`

- 原因分析：
  - myid 配置错误。

- 解决方法：

  - 重新编辑 `vim zoo.cfg`

    ```aidl
    vim zoo.cfg
    ```
  
    ```
    #######################cluster##########################
    
    server.1=hadoop102:2888:3888
    
    server.2=hadoop103:2888:3888
    
    server.3=hadoop104:2888:3888
    ```
  
  - 重新编辑 `/zkData` 目录下的 `myid` 文件：

    - hadoop102： 1；
    - hadoop103： 2；
    - hadoop104： 3。
    
  - 保存退出后重新启动 zookeeper。

#### 5.2. `zk.sh start` 时报错：`Zookeeper JAVA_HOME is not set and java could not be found in PATH`

- 原因分析：
  - `zkEnv.sh` 中未配置 JAVA_HOME。

- 解决方法：
  - 进入 `/usr/local/apache-zookeeper-3.5.7/bin` 目录
    ```
    cd /usr/local/apache-zookeeper-3.5.7/bin
    ```
  - 编辑 zkEnv.sh

    ```
    vim zkEnv.sh
    ```

  - 找到下面这段代码：

    ```
    if [[ -n "$JAVA_HOME" ]] && [[ -x "$JAVA_HOME/bin/java" ]];  then
    JAVA="$JAVA_HOME/bin/java"
    elif type -p java; then
    JAVA=java
    else
    echo "Error: JAVA_HOME is not set and java could not be found in PATH." 1>&2
    exit 1
    fi
    ```

  - 在上面这段代码前，添加 `JAVA_HOME`

    ```aidl
    JAVA_HOME="/usr/local/bin/jdk1.8"
    ```
  - 分发 `zkEnv.sh`

    ```
    xsync ./zkEnv.sh
    ```
