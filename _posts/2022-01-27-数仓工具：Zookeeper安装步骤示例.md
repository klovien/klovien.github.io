---
layout:     post
title:      数仓工具：Zookeeper安装步骤示例
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - 数仓
---

# 1. 下载安装

##### 1.1. 下载

<li>点击此处下载：<a href="https://yuyan.fandom.com/zh/wiki/%E4%B8%89%E5%8F%B6%E8%8D%89%E5%9B%BD%E9%99%85%E8%AF%AD">apache-zookeeper-3.5.7-bin.tar.gz</a></li>

##### 1.2. 安装

解压缩文件，到常用软件安装位置。

```aidl
tar -zxvf apache-zookeeper-3.5.7-bin.tar.gz
# 移动文件到指定位置
cp -r ./apache-zookeeper-3.5.7-bin/* /指定目录/zookeeper-3.5.7 & rm -rf apache-zookeeper-3.5.7-bin
```

# 2. 修改配置

##### 2.1. 配置 dataDir 和 myid

```aidl
mkdir zkData
cd zkData
```

- 修改 myid 为 2

```aidl
vim myid
```
```aidl
2
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

server.2=hadoop102:2888:3888

server.3=hadoop103:2888:3888

server.4=hadoop104:2888:3888

```

# 3. 运行

##### 3.1. 分发到 slave1 和 slave2

```aidl
sudo xsync zookeeper-3.5.7/ salve 1 2
```

##### 3.2. 修改 slave1 和 slave2 的设置

- slave1

```aidl
# 链接 slave1
ssh slave1

# 获取 root 权限
su

# 编辑 myid
vim /指定目录/zookeeper-3.5.7/zkData/myid

3

# 保存退出
```
- slave2

```aidl
# 链接 slave2
ssh slave2

# 获取 root 权限
su

# 编辑 myid
vim /指定目录/zookeeper-3.5.7/zkData/myid

4

# 保存退出
```

##### 3.3. 运行

```aidl
cd /zookeeper-3.5.7/bin
./zkServer.sh start

# 启动成功
ZooKeeper JMX enabled by default
Using config: /home/pandong/zookeeper-3.5.7/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
```
