---
layout:     post
title:      Mawell：Maxwell 功能介绍及使用示例
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Mawell
---


# 1. Maxwell 介绍

- Maxwell 是由美国 Zendesk 开源，使用 Java 编写的 MySQL 实时抓取工具，可以实时读取 MySQL 二进制日志 binlog，并生成 JSON 格式的消息，作为生产者发送给 Kafka，Kinesis、RabbitMQ、Redis、Google Cloud Pub/Sub、文件或其它平台的应用程序。
- Maxwell 设计的初衷是实时采集 Mysql 数据到 Kafka。支持全表load数据，支持自动断点还原，支持按照列将数据发送到Kafka不同分区。
- 
Maxwell官网：http://maxwells-daemon.io/

# 2. Maxwell工作原理

##### 2.1. Maxwell 把自己伪装成 MySQL 的 slave 从库

- Maxwell 工作原理与 Canal 工作原理一样，都是把自己伪装成 MySQL 的 slave 从库，同步 binlog 数据，来达到同步 MySQL 数据，与 Canal 相比，更加轻量。

- 使用 Maxwell，需要开启 MySQL binlog 日志。

  - 二进制日志（Binlog）是MySQL服务端非常重要的一种日志，它会保存MySQL数据库的所有数据变更记录。Binlog的主要作用包括主从复制和数据恢复。

##### 2.2. Maxwell & MySQL 主从复制

- Maxwell 的工作原理和主从复制密切相关。

- MySQL的主从复制，就是用来建立一个和主数据库完全一样的数据库环境，这个数据库称为从数据库。

- 主从复制的应用场景如下：
  - 做数据库的热备：
    - 主数据库服务器故障后，可切换到从数据库继续工作。
  - 读写分离：
    - 主数据库只负责业务数据的写入操作，而多个从数据库只负责业务数据的查询工作，在读多写少场景下，可以提高数据库工作效率。

- 主从复制的工作原理如下： 
  - Master主库将数据变更记录，写到二进制日志(binary log)中
  - Slave从库向mysql master发送dump协议，将master主库的binary log events拷贝到它的中继日志(relay log)
  - Slave从库读取并回放中继日志中的事件，将改变的数据同步到自己的数据库。

# 3. Maxwell 使用示例

- 版本
  - maxwells :     1.2.X 以上
  - mysql    :     5.1，5.5，5.6，5.7 
  - jdk       :     1.8 以上 
  - 操作系统  :     centos7

- Mysql 配置

  ```aidl
  [mysqld]
   
  #数据库id
  server-id = 1
  #启动binlog，该参数的值会作为binlog的文件名
  log-bin=mysql-bin
  #binlog类型，maxwell要求为row类型
  binlog_format=row
  #启用binlog的数据库，需根据实际情况作出修改
  binlog-do-db=gmall
  ```
- 创建库和用户

  ```
  创建库和用户以及相应的权限
  msyql> CREATE DATABASE maxwell;
  mysql> CREATE USER 'maxwell'@'%' IDENTIFIED BY 'maxwell';
  mysql> GRANT ALL ON maxwell.* TO 'maxwell'@'%';
  mysql> GRANT SELECT, REPLICATION CLIENT, REPLICATION SLAVE ON *.* TO 'maxwell'@'%';
  ```

- 配置maxwell
  ```
  cp config.properties.example config.properties
  ```
  
  ```
  #Maxwell数据发送目的地，可选配置有stdout|file|kafka|kinesis|pubsub|sqs|rabbitmq|redis
  producer=kafka
  #目标Kafka集群地址
  kafka.bootstrap.servers=hadoop102:9092,hadoop103:9092,hadoop104:9092
  #目标Kafka topic，可静态配置，例如:maxwell，也可动态配置，例如：%{database}_%{table}
  kafka_topic=maxwell
  
  #MySQL相关配置
  host=hadoop102
  user=maxwell
  password=maxwell
  jdbc_options=useSSL=false&serverTimezone=Asia/Shanghai
  ```

- Maxwell 启动关闭 .sh 脚本

  ```aidl
  #！/bin/bash

  MAXWELL_HOME=/usr/local/maxwell
  
  status_maxwell(){
  result=`ps -ef | grep maxwell | grep -v grep | wc -l`
  return $result
  }
  start_maxwell(){
  status_maxwell
  if [[ $? -eq 0 ]]; then
  echo "启动maxwell"
  $MAXWELL_HOME/bin/maxwell --config $MAXWELL_HOME/config.properties --daemon
  else
  echo "maxwell正在运行"
  fi
  }
  
  stop_maxwell(){
  status_maxwell
  if [[ $? -eq 1 ]]; then
  echo "关闭maxwell"
  ps -ef | grep maxwell | grep -v grep | awk '{print $2}' | xargs kill -9
  else
  echo "maxwell已经关闭"
  fi
  }
  case $1 in
  start )
  start_maxwell
  ;;
  stop )
  stop_maxwell
  ;;
  restart )
  stop_maxwell
  start_maxwell
  ;;
  esac
  ```

- 开启 kafka consumer

  ```aidl
  bin/kafka-console-consumer.sh --bootstrap-server hadoop102:9092 --topic maxwell
  ```

- Maxwell提供了bootstrap功能来进行历史数据的全量同步

  ```aidl
  {
  "database": "fooDB",
  "table": "barTable",
  "type": "bootstrap-start",
  "ts": 1450557744,
  "data": {}
  }
  {
  "database": "fooDB",
  "table": "barTable",
  "type": "bootstrap-insert",
  "ts": 1450557744,
  "data": {
  "txt": "hello"
    }
  }
  {
  "database": "fooDB",
  "table": "barTable",
  "type": "bootstrap-insert",
  "ts": 1450557744,
  "data": {
  "txt": "bootstrap!"
    }
  }
  {
  "database": "fooDB",
  "table": "barTable",
  "type": "bootstrap-complete",
  "ts": 1450557744,
  "data": {}
  }
  ```
- 注意：
  - 第一条 `type` 为 `bootstrap-start` 和最后一条 `type` 为 `bootstrap-complete` 的数据，是 `bootstrap` 开始和结束的标志，不包含数据，中间的 `type` 为 `bootstrap-insert` 的数据才包含数据。
  - bootstrap 输出的所有记录的ts都相同，为 bootstrap 开始的时间。

- maxwell 的json数据说明
  - http://maxwells-daemon.io/dataformat/
  
- 参考过滤配置：
  - http://maxwells-daemon.io/filtering/
- 