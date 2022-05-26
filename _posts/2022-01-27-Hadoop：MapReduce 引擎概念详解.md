---
layout:     post
title:      Hadoop：MapReduce 引擎概念详解
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hadoop
---

# 1. MapReduce 概念简介

#### 1.1. MapReduce 定义

- MapReduce 是一个软件框架，基于该框架能够容易地编写应用程序，这些应用程序能够运行在由上千个商用机器组成的大集群上，并以一种可靠的、具有容错能力的方式并行地处理上TB级别的海量数据集。
- 这个定义里面有着这些关键词：
    - 软件框架；
    - 并行处理；
    - 可靠且容错；
    - 大规模集群；
    - 海量数据集。

#### 1.2. MapReduce 的作用

- MapReduce 擅长处理大数据
    - Mapper 负责“分”，即把复杂的任务分解为若干个“简单的任务”来处理。
        >MapReduce的思想就是 **“分而治之”**。
        - “简单的任务”包含三层含义：
            - 一是数据或计算的规模相对原任务要大大缩小；
            - 二是就近计算原则，即任务会分配到存放着所需数据的节点上进行计算；
            - 三是这些小任务可以并行计算，彼此间几乎没有依赖关系。
        - Map 主要是 映射、变换、过滤的过程。
        - 一条数据进入map会被处理成多条数据，也就是 1 进 N 出。

    - Reducer 负责对 map 阶段的结果进行汇总。至于需要多少个 Reducer，用户可以根据具体问题，通过在 mapred-site.xml 配置文件里设置参数 mapred.reduce.tasks 的值，缺省值为1。
        - Reduce主要是 分解、缩小、归纳的过程。

#### 1.3. MapReduce 工作机制

![]({{site.baseurl}}/img-post/MapReduce-1.png)

MapReduce的整个工作过程如上图所示，它包含如下4个独立的实体：

- 实体一：客户端，用来提交 MapReduce 作业。
- 实体二：JobTracker，用来协调作业的运行。
- 实体三：TaskTracker，用来处理作业划分后的任务。
- 实体四：HDFS，用来在其它实体间共享作业文件。

# 2. MapReduce 知识全景

![]({{site.baseurl}}/img-post/MapReduce-4.png)

# 3. MapReduce 框架

#### 3.1. MapReduce 框架的组成

![]({{site.baseurl}}/img-post/MapReduce-2.png)

- 一个MapReduce作业通常会把输入的数据集切分为若干独立的数据块，由Map任务以完全并行的方式去处理它们。

- 框架会对Map的输出先进行排序，然后把结果输入给Reduce任务。通常作业的输入和输出都会被存储在文件系统中，整个框架负责任务的调度和监控，以及重新执行已经关闭的任务。

- 通常，MapReduce框架和分布式文件系统是运行在一组相同的节点上，也就是说，计算节点和存储节点通常都是在一起的。这种配置允许框架在那些已经存好数据的节点上高效地调度任务，这可以使得整个集群的网络带宽被非常高效地利用。`

- JobTracker
    - JobTracker负责调度构成一个作业的所有任务，这些任务分布在不同的TaskTracker上（由上图的JobTracker可以看到2 assign map 和 3 assign reduce）。你可以将其理解为公司的项目经理，项目经理接受项目需求，并划分具体的任务给下面的开发工程师。

- TaskTracker
    - TaskTracker负责执行由JobTracker指派的任务，这里我们就可以将其理解为开发工程师，完成项目经理安排的开发任务即可。

#### 3.2. MapReduce 的输入输出

- MapReduce借鉴了函数式语言中的思想，用Map和Reduce两个函数提供了高层的并行编程抽象模型。
    - Map: 对一组数据元素进行某种重复式的处理；
    - Reduce: 对Map的中间结果进行某种进一步的结果整理。

- MapReduce 中定义了如下的 Map 和 Reduce 两个抽象的编程接口，由用户去编程实现:
    - map: [k1,v1] → [(k2,v2)]
    - reduce: [k2, {v2,…}] → [k3, v3]

- MapReduce框架运转在<key,value>键值对上，也就是说，框架把作业的输入看成是一组<key,value>键值对，同样也产生一组<key,value>键值对作为作业的输出，这两组键值对有可能是不同的。

- 一个MapReduce作业的输入和输出类型如下图所示：可以看出在整个流程中，会有三组<key,value>键值对类型的存在。

![]({{site.baseurl}}/img-post/MapReduce-3.png)


#### 3.3. MapReduce 优点

- MapReduce 最大的亮点在于，通过抽象模型和计算框架，把需要做什么(what need to do)与具体怎么做(how to do)分开了，为程序员提供一个抽象和高层的编程接口和框架。
- 程序员仅需要关心其应用层的具体计算问题，仅需编写少量的处理应用本身计算问题的程序代码。
- 如何具体完成这个并行计算任务所相关的诸多系统层细节被隐藏起来，从分布代码的执行、到大到数千小到单个节点集群的自动调度使用，都交给计算框架去处理。


# 4. MapReduce 工作流程

![]({{site.baseurl}}/img-post/MapReduce-5.png)

#### 4.1. 分片、格式化数据源

- 输入 Map 阶段的数据源，必须经过分片和格式化操作。
    - 分片操作：
        - 指的是将源文件划分为大小相等的小数据块( Hadoop 2.x 中默认 128MB )，也就是分片( split )，Hadoop 会为每一个分片构建一个 Map 任务，并由该任务运行自定义的 map() 函数，从而处理分片里的每一条记录;
    - 格式化操作：
        - 将划分好的分片( split )格式化为键值对<key,value>形式的数据，其中， key 代表偏移量， value 代表每一行内容。

#### 4.2. 执行 MapTask

- 每个 Map 任务都有一个内存缓冲区(缓冲区大小 100MB )，输入的分片( split )数据经过 Map 任务处理后的中间结果会写入内存缓冲区中。  
- 如果写人的数据达到内存缓冲的阈值( 80MB )，会启动一个线程将内存中的溢出数据写入磁盘，同时不影响 Map 中间结果继续写入缓冲区。
- 在溢写过程中， MapReduce 框架会对 key 进行排序，如果中间结果比较大，会形成多个溢写文件，最后的缓冲区数据也会全部溢写入磁盘形成一个溢写文件，如果是多个溢写文件，则最后合并所有的溢写文件为一个文件。

#### 4.3. 执行 Shuffle 过程

- MapReduce 工作过程中， Map 阶段处理的数据如何传递给 Reduce 阶段，这是 MapReduce 框架中关键的一个过程，这个过程叫作 Shuffle 。
- Shuffle 会将 MapTask 输出的处理结果数据分发给 ReduceTask ，并在分发的过程中，对数据按 key 进行分区和排序。

#### 4.4. 执行 ReduceTask

- 输入 ReduceTask 的数据流是<key, {value list}>形式，用户可以自定义 reduce()方法进行逻辑处理，最终以<key, value>的形式输出。

#### 4.5. 写入文件

- MapReduce 框架会自动把 ReduceTask 生成的<key, value>传入 OutputFormat 的 write 方法，实现文件的写入操作。


# 5. MapTask

![]({{site.baseurl}}/img-post/MapReduce-7.png)

#### 5.1. Read 阶段

- MapTask 通过用户编写的 RecordReader ，从输入的 InputSplit 中解析出一个个 key / value 。

#### 5.2. Map 阶段

- 将解析出的 key / value 交给用户编写的 Map ()函数处理，并产生一系列新的 key / value 。

#### 5.3. Collect 阶段

- 在用户编写的 map() 函数中，数据处理完成后，一般会调用 outputCollector.collect() 输出结果，在该函数内部，它会将生成的 key / value 分片(通过调用 partitioner )，并写入一个环形内存缓冲区中(该缓冲区默认大小是 100MB )。

#### 5.4. Spill 阶段

- 即“溢写”，当缓冲区快要溢出时(默认达到缓冲区大小的 80 %)，会在本地文件系统创建一个溢出文件，将该缓冲区的数据写入这个文件。
- 将数据写入本地磁盘前，先要对数据进行一次本地排序，并在必要时对数据进行合并、压缩等操作。
- 写入磁盘之前，线程会根据 ReduceTask 的数量，将数据分区，一个 Reduce 任务对应一个分区的数据。
- 这样做的目的是为了避免有些 Reduce 任务分配到大量数据，而有些 Reduce 任务分到很少的数据，甚至没有分到数据的尴尬局面。
- 如果此时设置了 Combiner ，将排序后的结果进行 Combine 操作，这样做的目的是尽可能少地执行数据写入磁盘的操作。

#### 5.5. Combine 阶段

- 当所有数据处理完成以后， MapTask 会对所有临时文件进行一次合并，以确保最终只会生成一个数据文件

- 合并的过程中会不断地进行排序和 Combine 操作，
其目的有两个：一是尽量减少每次写人磁盘的数据量;二是尽量减少下一复制阶段网络传输的数据量。
- 最后合并成了一个已分区且已排序的文件。

# 6. ReduceTask

![]({{site.baseurl}}/img-post/MapReduce-6.png)

#### 6.1. Copy 阶段

- Reduce 会从各个 MapTask 上远程复制一片数据（每个 MapTask 传来的数据都是有序的），并针对某一片数据，如果其大小超过一定國值，则写到磁盘上，否则直接放到内存中

#### 6.2. Merge 阶段

- 在远程复制数据的同时， ReduceTask 会启动两个后台线程，分别对内存和磁盘上的文件进行合并，以防止内存使用过多或者磁盘文件过多。

#### 6.3. Sort 阶段

- 用户编写 reduce() 方法输入数据是按 key 进行聚集的一组数据。
- 为了将 key 相同的数据聚在一起， Hadoop 采用了基于排序的策略。
- 由于各个 MapTask 已经实现对自己的处理结果进行了局部排序，因此， ReduceTask 只需对所有数据进行一次归并排序即可。
- 为了将 key 相同的数据聚在一起， Hadoop 采用了基于排序的策略。
- 由于各个 MapTask 已经实现对自己的处理结果进行了局部排序，因此， ReduceTask 只需对所有数据进行一次归并排序即可。

#### 6.4. Reduce 阶段

- 对排序后的键值对调用 reduce() 方法，键相等的键值对调用一次 reduce()方法，每次调用会产生零个或者多个键值对，最后把这些输出的键值对写入到 HDFS 中

#### 6.5. Write 阶段

- reduce() 函数将计算结果写到 HDFS 上。

- 合并的过程中会产生许多的中间文件(写入磁盘了)，但 MapReduce 会让写入磁盘的数据尽可能地少，并且最后一次合并的结果并没有写入磁盘，而是直接输入到 Reduce 函数。
- 合并的过程中会产生许多的中间文件(写入磁盘了)，但 MapReduce 会让写入磁盘的数据尽可能地少，并且最后一次合并的结果并没有写入磁盘，而是直接输入到 Reduce 函数。


