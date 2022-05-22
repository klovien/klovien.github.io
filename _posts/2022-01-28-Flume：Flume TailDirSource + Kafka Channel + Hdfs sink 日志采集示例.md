---
layout:     post
title:      Flume：Flume TailDirSource + Kafka Channel + Hdfs sink 日志采集示例
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Flume
---


# 1. 模拟生成日志

##### 1.1. 日志生成脚本

- 下载脚本
  - 链接：https://pan.baidu.com/s/1OqriA6Yr5B-W90vR71RPkg?pwd=0qnm
  - 提取码：0qnm

- 将脚本复制到服务器；
- 修改日志存放路径
  ```
  vim logback.xml
  
  # 修改下面的路径到指定目录
  <property name="LOG_HOME" value="/usr/local/applog/log" />
  ```
  
##### 1.2. 修改日志的日期

- 修改要生成的日志的日期，具体的日期可以自行修改。

  ```aidl
  vim application.yml
  ```

  ```aidl
  #业务日期
  mock.date: "2021-06-08"
  ```

##### 1.3. 生成日志

- 后台运行

  ```aidl
  nohup java -jar gmall2020-mock-log-2021-01-22.jar 1>/dev/null 2>/dev/null &
  ```
  
##### 1.4. 群启日志生成脚本

- 在 `/usr/local/bin` 目录下编辑 `start_applog.sh`

  ```aidl
  #!/bin/bash
  for i in hadoop102 hadoop103 hadoop104;do
    echo ============ $i start applog ===================
    ssh $i "cd /usr/local/applog; nohup java -jar gmall2020-mock-log-2021-01-22.jar 1>/dev/null 2>/dev/null &"
  done
  ```

- 修改权限
  ```aidl
  chmod 777 start_applog.sh
  ```

- 分发日志生成脚本到 hadoop102 hadoop103 hadoop104
  ```
  xsync applog/
  ```
  
- 群启动
  ```aidl
  start_applog.sh 
  
  ============ hadoop102 start applog ===================
  ============ hadoop103 start applog ===================
  ============ hadoop104 start applog ===================
  ```

# 2. 采集日志

##### 2.1. 编辑 Flume 配置文件

- 在 `/flume/job` 目录下编辑 `file_to_kafka.conf`

  ```aidl
  a1.sources=r1
  a1.channels=c1 c2
  
  # configure source
  a1.sources.r1.type = TAILDIR
  a1.sources.r1.positionFile = /usr/local/flume/log_position.json
  a1.sources.r1.filegroups = f1
  a1.sources.r1.filegroups.f1 = /usr/local/applog/log/app.*
  a1.sources.r1.fileHeader = true
  a1.sources.r1.channels = c1 c2
  
  #interceptor
  a1.sources.r1.interceptors =  i1 i2
  a1.sources.r1.interceptors.i1.type = com.dex0423.flume.interceptor.ETLInterceptor$Builder
  a1.sources.r1.interceptors.i2.type = com.dex0423.flume.interceptor.ETLInterceptor$Builder
  
  a1.sources.r1.selector.type = multiplexing
  a1.sources.r1.selector.header = topic
  a1.sources.r1.selector.mapping.topic_start = c1
  a1.sources.r1.selector.mapping.topic_event = c2
  
  # configure channel
  a1.channels.c1.type = org.apache.flume.channel.kafka.KafkaChannel
  a1.channels.c1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092,hadoop104:9092
  a1.channels.c1.kafka.topic = topic_start
  a1.channels.c1.parseAsFlumeEvent = false
  a1.channels.c1.kafka.consumer.group.id = flume-consumer
  
  a1.channels.c2.type = org.apache.flume.channel.kafka.KafkaChannel
  a1.channels.c2.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092,hadoop104:9092
  a1.channels.c2.kafka.topic = topic_event
  a1.channels.c2.parseAsFlumeEvent = false
  a1.channels.c2.kafka.consumer.group.id = flume-consumer
  ```

##### 2.2. 编辑 Flume 拦截器

- 具体 JAVA 工程步骤，这里不再赘述，下面是两个核心文件。
  
  - <a href="{{site.baseurl}}/files/flume-interceptor.rar">点击下载站内资源：flume-interceptor.rar</a></li>

- `pom.xml`

  ```aidl
  <?xml version="1.0" encoding="UTF-8"?>
  <project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  
      <groupId>org.example</groupId>
      <artifactId>flume-interceptor</artifactId>
      <version>1.0-SNAPSHOT</version>
  
      <properties>
          <maven.compiler.source>8</maven.compiler.source>
          <maven.compiler.target>8</maven.compiler.target>
      </properties>
  
      <dependencies>
          <dependency>
              <groupId>org.apache.flume</groupId>
              <artifactId>flume-ng-core</artifactId>
              <version>1.9.0</version>
              <scope>provided</scope>
          </dependency>
  
          <dependency>
              <groupId>com.alibaba</groupId>
              <artifactId>fastjson</artifactId>
              <version>1.2.62</version>
          </dependency>
      </dependencies>
  
      <build>
          <plugins>
              <plugin>
                  <artifactId>maven-compiler-plugin</artifactId>
                  <version>2.3.2</version>
                  <configuration>
                      <source>1.8</source>
                      <target>1.8</target>
                  </configuration>
              </plugin>
              <plugin>
                  <artifactId>maven-assembly-plugin</artifactId>
                  <configuration>
                      <descriptorRefs>
                          <descriptorRef>jar-with-dependencies</descriptorRef>
                      </descriptorRefs>
                  </configuration>
                  <executions>
                      <execution>
                          <id>make-assembly</id>
                          <phase>package</phase>
                          <goals>
                              <goal>single</goal>
                          </goals>
                      </execution>
                  </executions>
              </plugin>
          </plugins>
      </build>
  
  
  </project>
  ```

- `ETLInterceptor.java`，我们在这里自己创建拦截器，根据业务不同可以创建不同的拦截器。

  ```aidl
  package com.dex0423.flume.interceptor;
  
  import org.apache.flume.Context;
  import org.apache.flume.Event;
  import org.apache.flume.interceptor.Interceptor;
  
  import java.nio.charset.StandardCharsets;
  import java.util.Iterator;
  import java.util.List;
  
  public class ETLInterceptor implements Interceptor {
  
  
      @Override
      public void initialize() {
  
      }
  
      @Override
      public Event intercept(Event event) {
  
          byte[] body = event.getBody();
  
          String log = new String(body, StandardCharsets.UTF_8);
  
          // 过滤 event 中的数据是否是 JSON 格式
          if (JSONUtils.isJSONValidate(log)){
              return event;
          }else {
              return null;
          }
      }
  
      @Override
      public List<Event> intercept(List<Event> list) {
  
          Iterator<Event> iterator = list.iterator();
  
          while (iterator.hasNext()) {
              Event next = iterator.next();
  
              if (intercept(next) == null){
                  iterator.remove();
              }
          }
  
          return list;
      }
  
      @Override
      public void close() {
  
      }
  
      public static class Builder implements Interceptor.Builder{
  
          @Override
          public Interceptor build() {
              return new ETLInterceptor();
          }
  
          @Override
          public void configure(Context context) {
  
          }
      }
  
  }
  ```

- build jar 包，推送到 `/usr/local/flume/lib` 目录下。

##### 2.3. 采集日志

- 确保 Hadoop 集群和 Kafka 正常运行。

- 在 hadoop103 `kafka/bin` 目录下创建 topic，挂一个消费者。

  ```aidl
  kafka-topics.sh --bootstrap-server hadoop102:9092 --alter --topic topic_log --partitions 3
  ```

- 在 hadoop102 启动 flume 将日志写入 kafka

  ```aidl
  bin/flume-ng agent --name a1 --conf-file /usr/local/flume/job/file_to_kafka.conf -Dflume.root.logger=info,console
  ```

[//]: # (- 如下即表示日志同步成功)

[//]: # (  ```aidl)

[//]: # (  ...)

[//]: # (  2022-05-21 18:02:22,016 INFO taildir.TaildirSource: r1 TaildirSource source starting with directory: {f1=/usr/local/applog/log/app.*})

[//]: # (  2022-05-21 18:02:22,020 INFO taildir.ReliableTaildirEventReader: taildirCache: [{filegroup='f1', filePattern='/usr/local/applog/log/app.*', cached=true}])

[//]: # (  2022-05-21 18:02:22,023 INFO taildir.ReliableTaildirEventReader: headerTable: {})

[//]: # (  2022-05-21 18:02:22,029 INFO taildir.ReliableTaildirEventReader: Opening file: /usr/local/applog/log/app.2022-05-17.log, inode: 1453474, pos: 0)

[//]: # (  2022-05-21 18:02:22,031 INFO taildir.ReliableTaildirEventReader: Updating position from position file: /usr/local/flume/log_position.json)

[//]: # (  2022-05-21 18:02:22,037 INFO taildir.TailFile: Updated position, file: /usr/local/applog/log/app.2022-05-17.log, inode: 1453474, pos: 3192719)

[//]: # (  2022-05-21 18:02:22,040 INFO instrumentation.MonitoredCounterGroup: Monitored counter group for type: SOURCE, name: r1: Successfully registered new MBean.)

[//]: # (  2022-05-21 18:02:22,040 INFO instrumentation.MonitoredCounterGroup: Component type: SOURCE, name: r1 started)

[//]: # (  2022-05-21 18:04:22,052 INFO taildir.TaildirSource: Closed file: /usr/local/applog/log/app.2022-05-17.log, inode: 1453474, pos: 3192719)

[//]: # (  ```)

# 3. 消费日志

- kafka_to_hdfs.conf

  ```aidl
  ## 组件
  a1.sources=r1 r2
  a1.channels=c1 c2
  a1.sinks=k1 k2
  
  ## source1
  a1.sources.r1.type = org.apache.flume.source.kafka.KafkaSource
  a1.sources.r1.batchSize = 5000
  a1.sources.r1.batchDurationMillis = 2000
  a1.sources.r1.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092,hadoop104:9092
  a1.sources.r1.kafka.topics=topic_start
  
  ## source2
  a1.sources.r2.type = org.apache.flume.source.kafka.KafkaSource
  a1.sources.r2.batchSize = 5000
  a1.sources.r2.batchDurationMillis = 2000
  a1.sources.r2.kafka.bootstrap.servers = hadoop102:9092,hadoop103:9092,hadoop104:9092
  a1.sources.r2.kafka.topics=topic_event
  
  ## channel1
  a1.channels.c1.type = file
  a1.channels.c1.checkpointDir = /usr/local/flume/checkpoint/behavior1
  a1.channels.c1.dataDirs = /usr/local/flume/data/behavior1/
  a1.channels.c1.maxFileSize = 2146435071
  a1.channels.c1.capacity = 1000000
  a1.channels.c1.keep-alive = 6
  
  ## channel2
  a1.channels.c2.type = file
  a1.channels.c2.checkpointDir = /usr/local/flume/checkpoint/behavior2
  a1.channels.c2.dataDirs = /usr/local/flume/data/behavior2/
  a1.channels.c2.maxFileSize = 2146435071
  a1.channels.c2.capacity = 1000000
  a1.channels.c2.keep-alive = 6
  
  ## sink1
  a1.sinks.k1.type = hdfs
  a1.sinks.k1.hdfs.path = /origin_data/gmall/log/topic_start/%Y-%m-%d
  a1.sinks.k1.hdfs.filePrefix = logstart-
  a1.sinks.k1.hdfs.round = true
  a1.sinks.k1.hdfs.roundValue = 10
  a1.sinks.k1.hdfs.roundUnit = second
  
  ## sink2
  a1.sinks.k2.type = hdfs
  a1.sinks.k2.hdfs.path = /origin_data/gmall/log/topic_event/%Y-%m-%d
  a1.sinks.k2.hdfs.filePrefix = logevent-
  a1.sinks.k2.hdfs.round = true
  a1.sinks.k2.hdfs.roundValue = 10
  a1.sinks.k2.hdfs.roundUnit = second
  
  ## 不要产生大量小文件
  a1.sinks.k1.hdfs.rollInterval = 10
  a1.sinks.k1.hdfs.rollSize = 134217728
  a1.sinks.k1.hdfs.rollCount = 0
  
  a1.sinks.k2.hdfs.rollInterval = 10
  a1.sinks.k2.hdfs.rollSize = 134217728
  a1.sinks.k2.hdfs.rollCount = 0
  
  ## 控制输出文件是原生文件。
  a1.sinks.k1.hdfs.fileType = CompressedStream 
  a1.sinks.k2.hdfs.fileType = CompressedStream 
  
  a1.sinks.k1.hdfs.codeC = lzop
  a1.sinks.k2.hdfs.codeC = lzop
  
  ## 拼装
  a1.sources.r1.channels = c1
  a1.sinks.k1.channel= c1
  
  a1.sources.r2.channels = c2
  a1.sinks.k2.channel= c2
  ```

# 4. 存储日志

- 日志处理一般采用批处理方式，而不是绝对的实时处理；
- 日志的存储，根据处理时间间隔，可以按 天\小时\30分钟\15分钟\1分钟 为单位创建目录，将日志文件存到对应的目录中，每次批处理的时候、处理一整个文件夹的日志文件；