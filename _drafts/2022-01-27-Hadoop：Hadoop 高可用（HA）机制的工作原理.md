---
layout:     post
title:      Hadoop：Hadoop 高可用（HA）机制的工作原理
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hadoop
---


https://blog.csdn.net/weixin_44825085/article/details/119645899






# 1. HDFS 概念

#### 1.1. HDFS 工作原理

- HDFS 概念
    - HDFS 是Hadoop Distribute File System 的简称，HDFS 是 Hadoop 生态的核心。
    - 它允许文件通过网络在多台主机上分享的文件系统，可以让多台机器上的多个用户分享文件和存储空间。
    >HDFS只是分布式文件管理系统中的一种。

    



HDFS和Yarn都是典型MS结构，即一个Master和多个Slave，这样就会产生单点故障，万一主节点挂掉了就不能对外提供服务，此时就需要配置我们的高可用，让集群可以7*24小时不间断的提供服务


#### NameNode 单点故障（SPOF） 

- NameNode 机器发生意外（如宕机），集群将无法使用，直到管理员重启；
- NameNode 机器需要升级（如软件、硬件升级），此时集群将无法使用；
- HDFS 配置 Active / Standby 两个 NameNode 实现集群主从热备，解决上述问题；
- 如果出现单点故障，可以迅速将 NameNode 切换到另外一台机器；

#### 如何保证三台 NameNode 数据一致

- FSImage
  - 让一台 NameNode 生成数据，其他机器的 NameNode 同步
- Edtis & JournalNode
  - 引进新的模块 JournalNode 保证 Edtis 文件的数据一致性

#### 如何实现同一时刻只有一台 Active，多台 Standby

- 手动分配
- 自动分配

#### 如何定期合并 FSImage 和 Edtis

- Standby 的 NameNode 来负责

#### NameNode 故障时，如何自动切换到 Standby 的 NameNode

- 手动故障转移
- 自动故障转移


#### JounalNode 规划

![]({{site.baseurl}}/img-post/hadoop-ha-1.png)


#### ZooKeeper

![]({{site.baseurl}}/img-post/hadoop-ha-2.png)



（1）hadoop-HA集群运作机制介绍

所谓HA，即高可用（7*24小时不中断服务），实现高可用最关键的是消除单点故障。
hadoop-ha严格来说应该分成各个组件的HA机制——HDFS的HA、YARN的HA

（2）HDFS的HA机制详解

通过双 namenode 消除单点故障，双 namenode 协调工作的要点：

A、元数据管理方式需要改变：
- 内存中各自保存一份元数据
- Edits日志只能有一份，只有Active状态的namenode节点可以做写操作
- 两个namenode都可以读取edits
- 共享的edits放在一个共享存储中管理（qjournal和NFS两个主流实现）

B、需要一个状态管理功能模块:
- 实现了一个zkfailover，常驻在每一个namenode所在的节点
- 每一个zkfailover负责监控自己所在namenode节点，利用zk进行状态标识
- 当需要进行状态切换时，由zkfailover来负责切换
- 切换时需要防止brain split现象的发生




