---
layout:     post
title:      Flume：Flume TailDirSource + Kafka Channel + Hdfs 日志采集示例
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

# TailDirSource

Kafka Channel 省去了 Sink