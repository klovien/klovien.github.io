---
layout:     post
title:      DolphinScheduler：DolphinScheduler 介绍
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Airflow
---

# DolphinScheduler 概念

#### DAG

- 全称 Directed Acyclic Graph，简称DAG。
- 工作流中的Task任务以有向无环图的形式组装起来，从入度为零的节点进行拓扑遍历，直到无后继节点为止。

#### 流程定义

- 通过拖拽任务节点并建立任务节点的关联所形成的可视化DAG；

#### 流程实例

- 流程实例是流程定义的实例化，可以通过手动启动或定时调度生成，流程定义每运行一次，产生一个流程实例；

#### 任务实例

- 任务实例是流程定义中任务节点的实例化，标识着具体的任务执行状态；

#### 任务类型

- 目前支持有SHELL、SQL、SUB_PROCESS(子流程)、PROCEDURE、MR、SPARK、PYTHON、DEPENDENT(依赖)，同时计划支持动态插件扩展，注意：其中子 SUB_PROCESS 也是一个单独的流程定义，是可以单独启动执行的；

#### 调度方式

- 支持基于 cron表 达式的定时调度和手动调度。命令类型支持：
  - 启动工作流
  - 从当前节点开始执行
  - 恢复被容错的工作流
  - 恢复暂停流程
  - 从失败节点开始执行
  - 补数
  - 定时
  - 重跑
  - 暂停
  - 停止
  - 恢复等待线程。
- 其中 恢复被容错的工作流 和 恢复等待线程 两种命令类型是由调度内部控制使用，外部无法调用；

#### 定时调度

- 采用 quartz 分布式调度器，并同时支持cron表达式可视化的生成；

#### 依赖

- 不单单支持 DAG 简单的前驱和后继节点之间的依赖，同时还提供任务依赖节点，支持流程间的自定义任务依赖；

#### 优先级

- 支持流程实例和任务实例的优先级，如果流程实例和任务实例的优先级不设置，则默认是先进先出；

#### 邮件告警

- 支持 SQL任务 查询结果邮件发送，流程实例运行结果邮件告警及容错告警通知；

#### 失败策略

- 对于并行运行的任务，如果有任务失败，提供两种失败策略处理方式，继续是指不管并行运行任务的状态，直到流程失败结束。结束是指一旦发现失败任务，则同时Kill掉正在运行的并行任务，流程失败结束；

#### 补数

- 补历史数据，支持区间并行和串行两种补数方式；

# DolphinScheduler 架构

#### DolphinScheduler 架构图

![]({{site.baseurl}}/img-post/dolphinscheduler-2.png)

#### MasterServer

- MasterServer 采用分布式无中心设计理念，MasterServer 主要负责 DAG 任务切分、任务提交监控，并同时监听其它 MasterServer 和 WorkerServer 的健康状态。

- MasterServer 服务启动时向 Zookeeper 注册临时节点，通过监听 Zookeeper 临时节点变化来进行容错处理。

- MasterServer 服务内主要包含:
  - Distributed Quartz 分布式调度组件：主要负责定时任务的启停操作，当 quartz 调起任务后， Master 内部会有线程池具体负责处理任务的后续操作;
  - MasterSchedulerThread：是一个扫描线程定时扫描数据库中的 command 表，根据不同的命令类型进行不同的业务操作;
  - MasterExecThread：主要是负责DAG任务切分、任务提交监控、各种不同命令类型的逻辑处理;
  - MasterTaskExecThread：主要负责任务的持久化;

#### WorkerServer

- WorkerServer也采用分布式无中心设计理念，WorkerServer主要负责任务的执行和提供日志服务。WorkerServer服务启动时向Zookeeper注册临时节点，并维持心跳。

- 该服务包含：
  - FetchTaskThread：主要负责不断从Task Queue中领取任务，并根据不同任务类型调用TaskScheduleThread对应执行器；
  - LoggerServer：是一个RPC服务，提供日志分片查看、刷新和下载等功能。

#### ZooKeeper

- ZooKeeper服务，系统中的MasterServer和WorkerServer节点都通过ZooKeeper来进行集群管理和容错。另外系统还基于ZooKeeper进行事件监听和分布式锁。 我们也曾经基于Redis实现过队列，不过我们希望EasyScheduler依赖到的组件尽量地少，所以最后还是去掉了Redis实现。

####  Task Queue

- 提供任务队列的操作，目前队列也是基于Zookeeper来实现。由于队列中存的信息较少，不必担心队列里数据过多的情况，实际上压测过百万级数据存队列，对系统稳定性和性能没影响。

####  Alert

- 提供告警相关接口，接口主要包括告警两种类型的告警数据的存储、查询和通知功能。其中通知功能又有邮件通知和 SNMP(暂未实现) 两种。

####  API

- API接口层，主要负责处理前端UI层的请求。该服务统一提供RESTful api向外部提供请求服务。 接口包括工作流的创建、定义、查询、修改、发布、下线、手工启动、停止、暂停、恢复、从该节点开始执行等等。

####  UI

- 系统的前端页面，提供系统的各种可视化操作界面。

![]({{site.baseurl}}/img-post/dolphinscheduler-1.png)





