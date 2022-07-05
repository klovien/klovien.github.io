---
layout:     post
title:      Spark：Spark 简介及工作原理
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Spark
---



# Spark

Spark 是一种通用、快速可扩展的大数据分析引擎；

Spark 解决了 MapReduce 读写磁盘速度过慢的问题；

Spark 框架下，不需要考虑数据倾斜问题，系统会自动优化；

Spark 既可以处理离线批处理、Impala 交互式分析、Storm 流失处理、机器学习、图计算；




# RDD 弹性分布式数据集

- Spark 处理数据时，将数据封装到 RDD 中，RDD 里会有很多 Partition 分区，每个分区数据被一个 Task 处理。

![]({{site.baseurl}}/img-post/spark-1.png)

- Saprk 和 Flink 的 task 是以线程方式运行的，而 MapReduce 是进程方式运行，线程运行速度快于进程，这是 Spark 速度更快的原因之一；
- 








