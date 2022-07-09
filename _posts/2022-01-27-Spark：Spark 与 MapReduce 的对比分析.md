---
layout:     post
title:      Spark：Spark 与 MapReduce 的对比分析
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Spark
---


#### 面向内存 VS 面向磁盘

- MapReduce 是面向磁盘的，受限于磁盘读/写性能和网络I/O性能的约束，在处理迭代计算、实时计算、交互式数据查询等方面并不高效，但是这些计算在图计算、数据挖掘和机器学习等相关应用领域中非常常见。
    - 针对 MapReduce 这一不足，将数据存储在内存中并基于内存进行计算是一个有效的解决途径。
    
- Spark 是面向内存的大数据处理引擎，这使得 Spark 能够为多个不同数据源的数据，提供近乎实时的处理性能，适用于需要多次操作特定数据集的应用场景。

- 在相同的实验环境下处理相同的数据：
    - 若在内存中运行，Spark 要比 MapReduce 快 100 倍；
    - 在磁盘中运行时，Spark 要比 MapReduce 快 10 倍；
    
- 综合各种实验表明：
    - 处理迭代计算问题 Spark 要比 MapReduce 快 20 多倍；
    - 计算数据分析类报表的速度，Spark 可提高 40 多倍；
    - Spark 能够在 5~7 秒的延时内交互式扫描 1TB 数据集。

![]({{site.baseurl}}/img-post/spark-2.png)

#### 简洁易用 VS 冗长代码

- 在使用 MapReduce 开发应用程序时，通常用户关注的重点与难点是如何将一个需求Job（作业）拆分成 Map 和 Reduce。
    - 由于 MapReduce 中仅为数据处理提供了两个操作，即 Map 和 Reduce，因此系统开发人员需要解决的一个难题是，如何把数据处理的业务逻辑合理有效地封装在对应的两个类中。
    - 以分词统计为例，虽然 MapReduce 固定的编程模式极大地简化了并行程序开发，但是代码至少几十行；
    
- 与之相对比，Spark提供了80多个针对数据处理的基本操作，如：map、flatMap、reduceByKey、filter、cache、collect、textFile 等；
    - 这使得用户基于 Spark 进行应用程序开发非常简洁高效，以分词统计为例，若换成Spark，其核心代码最短仅需一行，极大地提高了应用程序开发效率。
    - 基于Spark的WordCount程序核心代码：`sc.textFile("hdfs://master:8020/xxx/wc.input").flatMap(_.split("")).map((_,1)).reduceByKey(_ + _).collect`

#### 附加模块交互 VS Spark Sheep

- MapReduce 自身并没有交互模式，需要借助 Hive 和 Pig 等附加模块。
- Spark 提供了一种命令行交互模式，即 Spark Sheep，使得用户可以获取到查询和其他操作的即时反馈。

- 需要注意的是，在 Spark 的实际项目开发中多用 Scala 语言：
    - Scala 语言，约占 70%；
    - Java 语言，约占 20%；
    - Python 语言，约占10%。
    - Scala 通常使用方便、简洁的工具，其内部往往封装了更为复杂的机理，因此 Scala 与 Java 等语言比较起来，学习难度要大一些。

#### 数据格式和内存布局
     
- MapReduce Schema on Read 处理方式会引起较大的处理开销；
- Spark RDD 能支持粗粒度写操作，对于读操作则可以精确到每条 record，因此 RDD 可以用来作为分布式索引。此外用户可以自定义分区策略，如 Hash 分区等。

#### 任务调度对比

- Hadoop MapReduce的 Map Task 和 Reduce Task 都是进程级别的；

- Spark Task 是基于线程模型的，Spark 通过复用线程池中的线程来减少启动、关闭 task 所需要的开销。

#### 执行策略对比
     
- MapReduce 在 shuffle 前需要花费大量时间进行排序；
     
- Spark 在 shuffle 时只有部分场景才需要排序，支持基于 Hash 的分布式聚合，更加省时；


#### 方案的统一性

- 相对于 MapReduce，Spark 在方案的统一性方面，都有着极大的优势。
- Spark 框架包含了多个紧密集成的组件，位于底层的是Spark Core 实现了 Spark 的作业调度、内存管理、容错、与存储系统交互等基本功能，并针对弹性分布式数据集提供了丰富的操作。
- 在 Spark Core 的基础上，Spark 提供了一系列面向不同应用需求的组件，主要有 Spark SQL、Spark Streaming、MLlib、GraphX。
    - 这些 Spark 核心组件都以 jar 包的形式提供给用户，这意味着在使用这些组件时，无需进行复杂烦琐的学习、部署、维护和测试等一系列工作，用户只要搭建好 Spark 平台便可以直接使用这些组件，从而节省了大量的系统开发与运维成本。
    - 将这些组件放在一起，就构成了一个Spark软件栈。基于这个软件栈，Spark提出并实现了大数据处理的一种理念——“一栈式解决方案（one stack to rule them all）”，即Spark可同时对大数据进行批处理、流式处理和交互式查询，如图5所示。借助于这一软件栈用户可以简单而低耗地把各种处理流程综合在一起，充分体现了Spark的通用性。

#### Spark 比 MapReduce 快的原因

- Spark 是基于内存的大数据处理框架

    - Spark 既可以在内存中处理一切数据，也可以使用磁盘来处理未全部装入到内存中的数据。
    - 由于内存与磁盘在读/写性能上存在巨大的差距，因此 CPU 基于内存对数据进行处理的速度要快于磁盘数倍。
    - MapReduce 对数据的处理是基于磁盘展开的：
        - 一方面，MapReduce 对数据进行 Map 操作后的结果要写入磁盘中，而且 Reduce 操作也是在磁盘中读取数据；
        - 另一方面，分布式环境下不同物理节点间的数据通过网络进行传输，网络性能使得该缺点进一步被放大。
    - 因此，磁盘的读/写性能、网络传输性能成为了基于MapReduce大数据处理框架的瓶颈。


- Spark 具有优秀的作业调度策略

    - Spark 中使用了有向无环图（Directed Acyclic Graph，DAG）这一概念。
    - 一个 Spark 应用由若干个作业构成：
        - 首先，Spark 将每个作业抽象成一个图，图中的节点是数据集，图中的边是数据集之间的转换关系；
        - 然后，Spark 基于相应的策略将 DAG 划分出若干个子图，每个子图称为一个阶段，而每个阶段对应一组任务；
        - 最后，每个任务交由集群中的执行器进行计算。
    - 借助于 DAG，Spark 可以对应用程序的执行进行优化，能够很好地实现循环数据流和内存计算。






