---
layout:     post
title:      Kafka：Kafka 与 flume 的比较以及集成方法
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Kafka
---


# Kafka与Flume比较
在企业中必须要清楚流式数据采集框架flume和kafka的定位是什么：
flume：cloudera公司研发:
	适合多个生产者；
适合下游数据消费者不多的情况；
适合数据安全性要求不高的操作；
适合与Hadoop生态圈对接的操作。
kafka：linkedin公司研发:
适合数据下游消费众多的情况；
适合数据安全性要求较高的操作，支持replication。
因此我们常用的一种模型是：
线上数据 --> flume --> kafka --> flume(根据情景增删该流程) --> HDFS

# Flume与kafka集成
1）配置flume(flume-kafka.conf)
```
# define
a1.sources = r1
a1.sinks = k1
a1.channels = c1

# source
a1.sources.r1.type = exec
a1.sources.r1.command = tail -F -c +0 /opt/module/datas/flume.log
a1.sources.r1.shell = /bin/bash -c

# sink
a1.sinks.k1.type = org.apache.flume.sink.kafka.KafkaSink
a1.sinks.k1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092,hadoop104:9092
a1.sinks.k1.kafka.topic = first
a1.sinks.k1.kafka.flumeBatchSize = 20
a1.sinks.k1.kafka.producer.acks = 1
a1.sinks.k1.kafka.producer.linger.ms = 1

# channel
a1.channels.c1.type = memory
a1.channels.c1.capacity = 1000
a1.channels.c1.transactionCapacity = 100

# bind
a1.sources.r1.channels = c1
a1.sinks.k1.channel = c1
```

2） 启动kafkaIDEA消费者

3） 进入flume根目录下，启动flume

$ bin/flume-ng agent -c conf/ -n a1 -f jobs/flume-kafka.conf

4） 向 /opt/module/datas/flume.log里追加数据，查看kafka消费者消费情况

$ echo hello > /opt/module/datas/flume.log