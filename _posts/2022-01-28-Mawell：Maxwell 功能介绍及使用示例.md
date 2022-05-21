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
- Maxwell 设计的初衷是监听 Mysql 的 binlog 日志，实时采集 Mysql 数据，并且把解析的 json 格式数据发送到到 Kafka。
- Maxwell 支持全表 load 数据，支持自动断点还原，支持按照列将数据发送到 Kafka 不同分区。


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

##### 3.1. 环境 & 版本准备

- 版本
  - maxwells :     1.2.X 以上
  - mysql    :     5.1，5.5，5.6，5.7 
  - jdk       :     1.8 以上 
  - 操作系统  :     centos7

##### 3.2. Mysql 配置

- 编辑 conf

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

##### 3.3. 配置 Maxwell 

- 配置 `config.properties`
  ```
  cp config.properties.example config.properties
  ```
  
  ```
  # Maxwell数据发送目的地，可选配置有stdout|file|kafka|kinesis|pubsub|sqs|rabbitmq|redis
  producer=kafka
  #目标Kafka集群地址
  kafka.bootstrap.servers=hadoop102:9092,hadoop103:9092,hadoop104:9092
  #目标Kafka topic，可静态配置，例如:maxwell，也可动态配置，例如：%{database}_%{table}
  kafka_topic=maxwell
  
  # MySQL相关配置
  host=hadoop102
  user=maxwell
  password=maxwell
  jdbc_options=useSSL=false&serverTimezone=Asia/Shanghai
  ```

- `mysql options`
  - `host` 
    - 指定从哪个地址的mysql获取binlog

- `replication_host`
  - 如果指定了 `replication_host`，那么它是真正的 `binlog` 来源的 `mysql server` 地址，而那么上面的 `host` 用于存放 `maxwell` 表结构和 `binlog` 位置的地址。
  - 将两者分开，可以避免 replication_user 往生产库里写数据。
- `schema_host`
  - 从哪个 host 获取表结构。
  - binlog 里面没有字段信息，所以 maxwell 需要从数据库查出 schema，存起来。
  - schema_host 一般用不到，但在 binlog-proxy 场景下就很实用。
    - 比如：要将已经离线的 binlog 通过 maxwell 生成 json 流，由于 mysql server 里面没有结构，只用于发送 binlog，此时表机构就可以制动从 schema_host 获取。
- `gtid_mode`
  - 如果 mysql server 启用了 GTID，maxwell 也可以基于 gtid 取 event。
  - 如果 mysql server 发生 failover，maxwell 不需要手动指定 newfile:postion

>正常情况下，replication_host 和 schema_host都不需要指定，只有一个 --host。

- `schema_database`
  - 使用这个 db 来存放 maxwell 需要的表，比如要复制的 databases, tables, columns, postions, heartbeats。

- `filtering`
  - `include_dbs`
    - 只发送 binlog 里面这些 databases 的变更，以,号分隔，中间不要包含空格。
    - 也支持 java 风格的正则，如 include_tables=db1,/db\\d+/，表示 db1, db2, db3…这样的。（下面的filter都支持这种regex）
    - 提示：这里的dbs指定的是真实db。
    - 比如：
      - binlog里面可能 use db1 但 update db2.ttt，那么maxwell生成的json database 内容是db2。
  - `exclude_dbs`
    - 排除指定的这些 databbases
  - `include_tables`
    - 只发送这些表的数据变更。不只需要指定 database.
  - `exclude_tables`
    - 排除指定的这些表
  - `exclude_columns`
    - 不输出这些字段。
    - 如果字段名在 row 中不存在，则忽略这个 filter。
  - `include_column_values`
    - 1.12.0新引入的过滤项，只输出满足 `column=values` 的行；
    - 比如：
      - `include_column_values=bar=x,foo=y`，如果有 `bar` 字段，那么只输出值为 `x` 的行，如果有 `foo` 字段，那么只输出值为 `y` 的行。
  如果没有对应字段，如只有bar=x没有foo字段，那么也成立。（即不是 或，也不是 与）

  - `blacklist_dbs`
    - 一般不用。
    - `blacklist_dbs` 字面上难以与 `exclude_dbs` 分开，官网的说明也是模棱两可。

  - 注意这些 `include` 与 `exclude` 的关系，记住三点：
    - 只要 include 有值，那么不在include里面的都排除
    - 只要在 exclude 里面的，都排除
    - 其它都正常输出

- `formatting`
  - `output_ddl`
    - 是否在输出的 json 流中，包含 ddl 语句，默认 false。
  - `output_binlog_position`
    - 是否在输出的 json 流中，包含 binlog filename:postion，默认 false。
  - `output_commit_info`
    - 是否在输出的 json 流里面，包含 commit 和 xid 信息，默认 true。
    - 比如一个事物里，包含多个表的变更，或一个表上多条数据的变更，那么他们都具有相同的 xid，最后一个row event输出 commit:true 字段。这有利于消费者实现 事务回放，而不仅仅是行级别的回放。
  - `output_thread_id`
    - binlog 里面也包含了 thread_id ，可以包含在输出中，默认 false。
    - 消费者可以用它来实现更粗粒度的事务回放。还有一个场景是用户审计，用户每次登陆之后将登陆ip、登陆时间、用户名、thread_id记录到一个表中，可轻松根据thread_id关联到binlog里面这条记录是哪个用户修改的。
  - `monitoring`
    - 如果是长时间运行的 maxwell，添加 monitor 配置，maxwell 提供了 http api 返回监控数据。

- `init_position`
  - 手动指定 maxwell 要从哪个 binlog，哪个位置开始。
  - 指定的格式 `FILE:POSITION:HEARTBEAT`。
  - 只支持在启动maxwell的命令指定，比如 `--init_postion=mysql-bin.0000456:4:0`。
  - maxwell 默认从连接上 mysql server 的当前位置开始解析，如果指定 init_postion，要确保文件确实存在，如果 binlog 已经被 purge 掉了，可能需要想其它办法。

##### 3.4. Maxwell 启动关闭

- 启动关闭 .sh 脚本

  ```aidl
  #!/bin/bash

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

##### 3.5. 使用 kafka 作为消息队列

- kafka是maxwell支持最完善的一个producer，并且内置了 多个版本的 kafka client(0.8.2.2, 0.9.0.1, 0.10.0.1, 0.10.2.1 or 0.11.0.1)，默认 kafka_version=0.11.0.1

- 开启 kafka consumer

  ```aidl
  bin/kafka-console-consumer.sh --bootstrap-server hadoop102:9092 --topic maxwell
  ```

- Maxwell提供了 bootstrap 功能来进行历史数据的全量同步

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
