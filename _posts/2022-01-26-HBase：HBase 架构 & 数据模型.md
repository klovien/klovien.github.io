---
layout:     post
title:      HBase：HBase 架构 & 数据模型
subtitle:   
date:       2022-01-26
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - HBase
---


# 1. HBase 架构

#### 1.1. 列式存储

- 关于本节，可参考文章 《<a href="https://dex0423.github.io/2022/01/26/OLAP-%E8%A1%8C%E5%BC%8F%E5%AD%98%E5%82%A8-&-%E5%88%97%E5%BC%8F%E5%AD%98%E5%82%A8/">OLAP：行式存储 & 列式存储</a>》。

#### 1.2. HBase 架构图

![]({{site.baseurl}}/img-post/hbase-1.png)

#### 1.3. Client 客户端

  - Client 提供了访问 HBase 的接口；
  - Client 维护了对应的 cache 来加速 HBase 的访问；

#### 1.4. Zookeeper 

  - 存储 HBase 的元数据（meta 表）；
  - 无论是读还是写数据，都是去 Zookeeper 里边拿 meta 元数据；、
  - Zookeeper 告诉给客户端去哪台机器读写数据；

#### 1.5. HRegionServer

  - HRegionServer 处理客户端的读写请求；
  - 负责与 HDFS 底层交互，是真正干活的节点。

  - HBase 之所以能存储海量的数据，是因为 HBase 是分布式的。
  - HBase一张表的数据会分到多台机器上的。那HBase是怎么切割一张表的数据的呢？用的就是RowKey来切分，其实就是表的横向切割。
  - 一个HRegion上，存储HBase表的一部分数据。
  - 
    ![]({{site.baseurl}}/img-post/hbase-7.png)

#### 1.6. HMaster

  - HMaster 会处理元数据的变更、监控 RegionServer 的状态、处理 HRegion 的分配或转移；
  - 如果我们 HRegion 的数据量太大的话，HMaster 会对拆分后的 Region 重新分配 RegionServer；
  - 如果发现失效的 HRegion，也会将失效的 HRegion 分配到正常的 HRegionServer 中；

#### 1.3. HBase 数据流程

![]({{site.baseurl}}/img-post/hbase-2.png)

- client 请求到 Zookeeper；
- Zookeeper 查找 meta 元数据，返回 HRegionServer 地址给 client；
- client 得到 Zookeeper 返回的地址去请求 HRegionServer；
- HRegionServer 读写数据，并返回结果给 client。

#### 1.4. HRegionServer



# 2. HBase 数据模型

#### 2.1. HBase 列族 & 列

- HBase 里边也有表、行和列的概念
  - HBase 一行数据，由一个行键(RowKey)、一个或多个相关的列以及它的值所组成；
  - 在 HBase 里边，定位一行数据会有一个唯一的值，这个叫做行键(RowKey)。
  
- HBase 的列，不是我们在关系型数据库所想象中的列
  - HBase 的列（Column）归属到列族（Column Family）中；
  - 在 HBase 中用列修饰符（Column Qualifier）来标识每个列；

- 在 HBase 里，先有列族，后有列，在列族下用列修饰符来标识一列。

  ![]({{site.baseurl}}/img-post/hbase-3.png)

- HBase表的每一行中，列的组成都是灵活的，行与行之间的列不需要相同。

  ![]({{site.baseurl}}/img-post/hbase-4.png)

- 一个列族下可以任意添加列，不受任何限制。

  ![]({{site.baseurl}}/img-post/hbase-5.png)

#### 2.2. HBase 数据时间戳

- 数据写到 HBase 的时候，都会被记录一个时间戳，这个时间戳被我们当做一个版本；
  - 比如：我们修改或者删除某一条的时候，本质上是往里边新增一条数据，记录的版本加一了而已。

    ![]({{site.baseurl}}/img-post/hbase-6.png)

#### 2.3. HBase Key-Value 结构

- Key
  - 由 RowKey(行键)+ColumnFamily（列族）+Column Qualifier（列修饰符）+TimeStamp（时间戳--版本）+KeyType（类型）组成
- Value
  - Value 就是实际上的值。




