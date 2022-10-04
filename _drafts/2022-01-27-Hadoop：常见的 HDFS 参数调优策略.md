---
layout:     post
title:      Hadoop：常见的 HDFS 参数调优策略
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - 数仓
---

# 1. hadoop运行环境

![]({{site.baseurl}}/img-post/hadoop调优-1.png)

# 2. 硬件选择

#### 2.1. 主节点可靠性要好于从节点

#### 2.2. 多路多核，高频率cpu、大内存，
- namenode 100万文件的元数据要消耗800M内存，内存决定了集群保存文件数的总量， resourcemanager同时运行的作业会消耗一定的内存。
- datanode 的内存需要根据cpu的虚拟核数(vcore) 进行配比，CPU的vcore数计算公式为=cpu个数 * 单cpu核数* HT（超线程）
- 内存容量大小 = vcore数 * 2GB(至少2GB)

#### 2.3. 根据数据量确定集群规模
- 一天增加10GB, 365天，原数据1TB，replacation=3，1.3 mapreduce 计算完保存的数据，规划容量 （1TB + 10GB*365）*3 * 1.3 =17.8TB，如果一台datanode的存储空间为2TB，18/2= 9，总节点为 = 9+2 =11 ，还要考虑作业并不是均匀分布的；
- 有可能会倾斜到某一个时间段，需要预留资源

#### 2.4. 不要让网络 I/O 成为瓶颈
- hadoop 作业通常是 I/O密集型而非计算密集型， 瓶颈通常集中出现在I/O上；
- 计算能力可以通过增加新节点进行线性扩展，要注意网络设别处理能力。

# 3. 操作系统

#### 3.1. 避免使用 swap 分区

- 将 hadoop 守护进程的数据交换到硬盘的行为，可能会导致操作超时。

#### 3.2. 调整内存分配策略

- 操纵系统内核根据 vm.oversommit_memory 的值来决定分配策略；
- 并且通过vm.overcommit_ratio的值来设定超过物理内存的比例。

#### 3.3. 修改 net.core.somaxconn 参数

- 该参数表示socker监听backlog的上限，默认为128,socker的服务器会一次性处理backlog中的所有请求，hadoop的ipc.server.listen.queue.size参数和linux的net.core.somaxconn
- 参数控制了监听队列的长度，需要调大。

#### 3.4. 增大同时打开文件描述符的上限

- 对内核来说，所有打开的文件都通过文件描述符引用，文件描述符是一个非负整数；
- hadoop的作业经常会读写大量文件，需要增大同时打开文件描述符的上限。

#### 3.5. 选择合适的文件系统，并禁用文件的访问时间

- 文件访问时间可以让用户知道那些文件近期被查看或修改；
- 但对hdfs来说，获取某个文件的某个块被修改过没有意义，可以禁用。

#### 3.6. 关闭THP (transparent Huge Pages)

- THP 是一个使管理 Huge Pages自动化的抽象层， 它会引起cpu占用率增大， 需要关闭。

  ```aidl
  echo never > /sys/kernel/mm/redhat_transparent_hugepage/defrag
  echo never > /sys/kernel/mm/redhat_transparent_hugepage/enabled
  echo never > /sys/kernel/mm/transparent_hugepage/enabled
  echo never > /sys/kernel/mm/transparent_hugepage/defrag
  ```



# 4. HDFS 调优

#### 4.1. dfs.block.size
  - 设置合理的块大小；

#### 4.2. mapred.local.dir
  - 将中间结果目录设置为分布在多个硬盘，以提升写入速度；

#### 4.3. dfs.datanode.handler.count
  - 该值用于设置datanode处理RPC的线程数，,默认为3；
  - 大集群可以适当加大（dfs.datanode.handler.count），可以适当加大为 10。

#### 4.4. dfs.namenode.handler.count
  - NameNode有一个工作线程池，用来处理不同DataNode的并发心跳以及客户端并发的元数据操作，参数dfs.namenode.handler.count的默认值10；
  - 对于大集群或者有大量客户端的集群来说，通常需要增大该值的设置，设置该值的一般原则是将其设置为集群大小的自然对数乘以20，即20logN，N为集群大小；
  - 计算公式：dfs.namenode.handler.count=20 * log2(Cluster Size)；
  - 比如集群规模为 8台 时，此参数设置为 60。

#### dfs.datanode.data.dir 
  - HDFS数据存储目录;
  - 将数据存储分布在各个磁盘上可充分利用节点的I/O读写性能。


#### 日志存储路径 & 镜像文件存储路径
- 编辑日志存储路径dfs.namenode.edits.dir设置与镜像文件存储路径dfs.namenode.name.dir尽量分开，达到最低写入延迟，提高集群的读写方面性能

#5. YARN 调优

- yarn的资源表示模型为 ceontainer（容器），container 将资源抽象为两个维度，内存和虚拟cpu(vcore)
  - 兼容各种计算框架；
  - 动态分配资源，减少资源浪费；

#### 5.1. 容器内存

- yarn.nodemanager.resource.memory-mb
  - 表示该节点上YARN可使用的物理内存总量，默认是8192（MB）；
  - 如果你的节点内存资源不够 8GB，则需要调减小这个值；
- 注意：
  - YARN 不会智能的探测节点的物理内存总量，必须手动修改；
  - 如果不修改会导致内存使用率过低，洪峰过来时集群会挂掉。
- 调优方法：
  - 根据电脑内存来调节；
  - 比如：
    - 电脑内存是 128G 的，那这里的值就调成 100G。

#### 4.7. 最小容器内存

- yarn.scheduler.minimum-allocation-mb
  - 表示单个任务可申请的最多物理内存容量，默认是 8192(MB)；
  - MR 过程并不吃内存，所以这里一般不需要修改，除非代码写的太烂；
  - 如果确有需要，调大到 16G 就可以。

#### 4.8. 容器内存增量
- yarn.scheduler.increment-allocation-mb

#### 4.9. 最大容器内存
- yarn.scheduler.maximum-allocation-mb
- 单个任务可申请的最多物理内存量，默认是8192（MB）。

#### 4.10. 容器虚拟cpu内核
- yarn.nodemanager.resource.cpu-vcores

#### 4.11. 最小容器虚拟cpu内核数量
- yarn.scheduler.minimum-allocation-vcores

#### 4.12. 容器虚拟cpu内核增量
- yarn.scheduler.increment-allocation-vcores

#### 4.13. 最大容器虚拟cpu内核数量
- yarn.scheduler.maximum-allocation-vcores

#### 4.14. MapReduce调优

- 三大原则：
  - 增大作业并行程度
  - 给每个任务足够的资源 
  - 在满足前2个条件下，尽可能的给shuffle预留资源
  
#### 示例：

情景描述：总共7台机器，每天几亿条数据，数据源->Flume->Kafka->HDFS->Hive

面临问题：数据统计主要用HiveSQL，没有数据倾斜，小文件已经做了合并处理，开启的JVM重用，而且IO没有阻塞，内存用了不到50%。但是还是跑的非常慢，而且数据量洪峰过来时，整个集群都会宕掉。基于这种情况有没有优化方案。

（2）解决办法：

内存利用率不够。这个一般是Yarn的2个配置造成的，单个任务可以申请的最大内存大小，和Hadoop单个节点可用内存大小。调节这两个参数能提高系统内存的利用率。

（a）yarn.nodemanager.resource.memory-mb
（b）yarn.scheduler.maximum-allocation-mb

