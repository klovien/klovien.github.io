---
layout:     post
title:      OLAP：HBase vs RDBMS vs HDFS vs Hive
subtitle:   
date:       2022-01-26
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - HBase
---


# RDBMS vs HBase

#### 关系型数据库

- 结构
  - 数据库以表的形式存在
  - 
  - 支持FAT、NTFS、EXT、文件系统
  - 使用主键（PK）
  - 通过外部中间件可以支持分库分表，但底层还是单机引擎
  - 使用行、列、单元格
- 功能
  - 支持向上扩展（买更好的服务器）
  - 使用SQL查询
  - 面向行，即每一行都是一个连续单元
  - 数据总量依赖于服务器配置
  - 具有ACID支持
  - 适合结构化数据
  - 传统关系型数据库一般都是中心化的
  - 支持事务
  - 支持Join
  
#### HBase

- 结构
  - 以表形式存在
  - 支持HDFS文件系统
  - 使用行键（row key）
  - 原生支持分布式存储、计算引擎
  - 使用行、列、列蔟和单元格
- 功能
  - 支持向外扩展
  - 使用API和MapReduce、Spark、Flink来访问HBase表数据
  - 面向列蔟，即每一个列蔟都是一个连续的单元
  - 数据总量不依赖具体某台机器，而取决于机器数量
  - HBase不支持ACID（Atomicity、Consistency、Isolation、Durability）
  - 适合结构化数据和非结构化数据
  - 一般都是分布式的
  - HBase不支持事务，支持的是单行数据的事务操作
  - 不支持Join

# HDFS vs HBase

#### HDFS
  
  - HDFS是一个非常适合存储大型文件的分布式文件系统
  - HDFS它不是一个通用的文件系统，也无法在文件中快速查询某个数据

#### HBase
  
  - HBase构建在HDFS之上，并为大型表提供快速记录查找(和更新)
  - HBase内部将大量数据放在HDFS中名为「StoreFiles」的索引中，以便进行高速查找
  - Hbase比较适合做快速查询等需求，而不适合做大规模的OLAP应用

# Hive vs Hbase

#### Hive
  - >Hive 没有存储引擎，也没有计算引擎
  - 数据仓库工具
Hive的本质其实就相当于将HDFS中已经存储的文件在Mysql中做了一个双射关系，以方便使用HQL去管理查询
  - 用于数据分析、清洗
Hive适用于离线的数据分析和清洗，延迟较高
  - 基于HDFS、MapReduce
Hive存储的数据依旧在DataNode上，编写的HQL语句终将是转换为MapReduce代码执行

#### HBase

- NoSQL数据库 
  - 是一种面向列存储的非关系型数据库。
- 用于存储结构化和非结构化的数据 
  - 适用于单表非关系型数据的存储，不适合做关联查询，类似JOIN等操作。
- 基于HDFS 
  - 数据持久化存储的体现形式是Hfile，存放于DataNode中，被ResionServer以region的形式进行管理
- 延迟较低，接入在线业务使用 
  - 面对大量的企业数据，HBase可以直线单表大量数据的存储，同时提供了高效的数据访问速度

#### Hive vs HBase

- Hive 和 Hbase 是两种基于 Hadoop 的不同技术
- Hive 是一种类 SQL 的引擎，并且运行 MapReduce 任务
- Hbase 是一种在 Hadoop 之上的 NoSQL 的 Key/value 数据库
- 这两种工具是可以同时使用的。
  - 就像用 Google 来搜索，用 FaceBook 进行社交一样；
  - Hive 可以用来进行统计查询，HBase 可以用来进行实时查询；
  - 数据也可以从 Hive 写到 HBase，或者从 HBase 写回 Hive。