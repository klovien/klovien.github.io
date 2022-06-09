---
layout:     post
title:      Hadoop：YARN 性能参数调优
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hadoop
---


# YARN 的概念

- YARN 是一个通用的资源管理系统和调度平台，可以为上层提供统一的资源管理和调度；

- 资源管理系统
  - 集群的硬件资源，和程序运行相关，比如内存、CPU；
- 调度平台
  - 解决多个程序同时申请计算资源，如何分配、调度规则等问题；

>YARN 不管理 磁盘，磁盘是由 HDFS 管理。

- 可以把 YARN 理解为一个分布式操作系统平台，而 MapReduce 等计算程序，则相当于运行操作系统之上的应用程序。


- YARN 具有良好的通用性和包容性：

- 正是因为有了 YARN，更多的计算框架可以接入到 HDFS 中，不仅是 MapReduce，Flink、Spark 都可以顺利接入 HDFS。
- 正是因为 YARN 的包容性，是的其他的计算框架可以专注于计算性能的提升。


![]({{site.baseurl}}/img-post/yarn-1.png)

# YARN 的作用

YARN 使得集群具备以下优点：

- 可扩展性：
  - 可以平滑地扩展至数万节点和并发的应用
- 可维护性：
  - 保证集群软件的升级与用户应用程序完全解耦
- 多租户：
  - 支持同一集群中多个租户并存，同时支持多个租户间细颗粒度地共享单个节点
- 位置感知：
  - 将计算移至数据所在位置
- 高集群使用率：
  - 实现底层物理资源的高使用率
- 安全和可审计的操作：
  - 以安全、可审计的方式使用集群资源
- 可靠性和可用性：
  - 具有高度可靠的用户交互、并支持高可用性
- 对编程模型多样化的支持：
  - 不仅支持mapreduce，还支持其他模型
- 灵活的资源模型：
  - 支持各个节点的动态资源配置以及灵活的资源模型
- 向后兼容：
  - 保持现有的mapreduce应用程序的向后兼容性。


# YARN 三大组件

#### ResourceManager（RM）

- YARN 集群中的主角色，决定系统中所有应用程序之间资源分配的最终权限；
  - 调度器根据容量、队列等限制条件（如每个队列分配一定的资源，最多执行一定数量的作业等），将系统中的资源分配给各个正在运行的应用程序。
  - 需要注意的是，该调度器是一个“纯调度器”，它不再从事任何与具体应用程序相关的工作，比如：
    - RM 不负责监控或者跟踪应用的执行状态等，
    - RM 也不负责重新启动因应用执行失败或者硬件故障而产生的失败任务，
  - 这些均交由应用程序相关的 ApplicationMaster 完成。
- RM 调度器仅根据各个应用程序的资源需求进行资源分配，而资源分配单位用一个抽象概念“资源容器”（Resource Container，简称 Container）表示，Container 是一个动态资源分配单位，它将内存、CPU、磁盘、网络等资源封装在一起，从而限定每个任务使用的资源量。
- 此外，RM 该调度器是一个可插拔的组件，用户可根据自己的需要设计新的调度器，YARN 提供了多种直接可用的调度器，比如 Fair Scheduler和Capacity Scheduler 等。
- 接收用户的作业提交，并通过 NodeManger 分配、管理各个机器上的计算资源；

#### NodeManager（NM）

- YARN 中的从角色，一台机器上一个，负责管理机器上的计算资源；
- 根据 RM 命令，启动 Container 容器、监视容器的资源使用情况，并向 RM 主角色汇报资源使用情况；


#### ApplicationMaster（AM）

- 用户提交的每个应用程序，均包含一个 AM；
- 应用程序内的“老大”，负责程序内部各个阶段的资源申请，监督程序的执行情况；
- AM 与 RM 调度器协商以获取资源（资源用 Container 表示）；
- 将得到的任务进一步分配给内部的任务（资源的二次分配）；
- 与 NM 通信以启动 / 停止任务；
- 监控所有任务运行状态，并在任务运行失败时重新为任务申请资源以重启任务。
- 当前 YARN 自带了两个AM实现，一个是用于演示AM编写方法的实例程序 distributedshell，它可以申请一定数目的 Container 以并行运行一个 Shell 命令或者Shell脚本；另一个是运行 MapReduce 应用程序的AM—MRAppMaster。

#### Container

- Container 是 YARN 中的资源抽象，它封装了某个节点上的多维度资源，如内存、CPU、磁盘、网络等，当AM向 RM 申请资源时，RM 为 AM 返回的资源便是用 Container 表示。
- YARN 会为每个任务分配一个 Container，且该任务只能使用该 Container 中描述的资源。

# YARN 核心交互流程

#### 核心流程

- MR 作业提交  client -> RM
- 资源申请  MRAppMaster -> RM
- MR 作业状态汇报  Container（ Map | Reduce Task ） -> Container（MRAppMaster）
- 节点的状态汇报 NM -> RM

#### 具体步骤

- 当用户向yarn中提交一个应用程序后，yarn将分两个阶段运行该应用程序。
  - 第一个阶段是启动 ApplicationMaster。
  - 第二个阶段是由 Applicationmaster 刨建应用程序，为它申请资源，并监控它的整个运行过程，直到运行完成。

- 第1步：用户向yarn中提交应用程序，其中包括ApplicationMaster程序、启动 ApplicationMaster的命令、用户程序等。
- 第2步：Resourcemanager为该应用程序分配第一个Container，并与对应的 NodeManager通信，要求它在这个Container中启动应用程序的ApplicationMaster。
- 第3步：Applicationmaster首先向ResourceManager注册，这样用户可以直接通过 Resourcemanager查看应用程序的运行状态，然后它将为各个任务申请资源，并监控它的运行状态，直到运行结束。即重复步骤4~7。
- 第4步：ApplicationMaster通过RPC协议向Resourcemanager申请和领取资源。
- 第5步：一旦ApplicationMaster申请到资源后，便与对应的Nodemanager通信，要求它启动任务。
- 第6步：Nodemanager为任务设置好运行环境后(包括环境变量、JAR包、二进制程序等)，将任务启动命令写到一个脚本中，并通过运行该脚本启动任务。
- 第7步：各个任务通过RPC协议向ApplicationMaster汇报自己的状态和进度，让 ApplicationMaster随时掌握各个任务的运行状态，从而可以在任务失败时重新启动任务。在应用程序运行过程中，用户可随时通过RPC向Applicationmaster查询应用程序的当前运行状态。
- 第8步：应用程序运行完成后，ApplicationMaster向Resourcemanager注销并关闭自己。

# YARN 的 RM 重启机制
- ResourceManager负责资源管理和应用的调度，是yarn的核心组件，存在单点故障的问题。
- ResourceManager restart重启机制是使RM在重启动时能够使yarn集群继续正常工作，并且使RM出现的失败不被用户知道。 
- 重启机制需要借助zookeeper集群来存储信息实现。
- 重启机制并不是自动帮我们重启的意思，所以不能解决单点故障问题。

- 不开启RM重启机制。RM故障重启后，之前集群上跑的任务信息都会消失，正在执行的作业也会失败。
- 
- RM重启机制有两种，一种是保留工作的重启机制，另一种是不保留工作的重启机制。
  - 不保留工作的RM重启机制。只保存了application提交的信息和最终执行状态，并不保存运行过程中的相关数据，所以RM重启后，会先杀死正在执行的任务，再重新提交，从零开始执行任务。
  - 保留工作的重启机制。保存了application运行中的状态数据，所以在RM重启之后，不需要杀死之前的任务，而是接着原来执行到的进度继续执行。

# YARN 的高可用性架构
- ResourceManager 负责资源管理和应用的调度，是yarn的核心组件，集群的主角色，存在单点故障问题。为了解决RM的单点故障问题，yarn设计了一套Active/Standby模式的ResourceManager HA架构。
- HA架构能够保证运行的ResourceManager挂掉后备用的ResourceManager能迅速接替工作，保证计算任务不会中断，转换过程对用户无感。
- 该HA架构也使用zookeeper集群实现。

- 实现HA集群的关键是：
  - 主备之间状态数据同步、主备之间顺利切换（故障转移机制）。
- 针对数据同步问题，可以通过zookeeper来存储共享集群的状态数据，因为zookeeper本质也是一个小文件存储系统。
- 针对主备顺利切换，也可以基于zookeeper自动实现。


# YARN 的通信协议
- 分布式环境下，需要涉及跨机器跨网络通信，yarn底层使用RPC协议实现通信。

- RPC是远程过程调用( Remote procedure call)的缩写形式。
- 基于RPC进行远程调用就像本地调用一样。
- 在RPC协议中，通信双方有一端是Client，另一端为Server，且Client总是主动连接Server的。因此，yarn实际上采用的是拉式(pull-based)通信模型。