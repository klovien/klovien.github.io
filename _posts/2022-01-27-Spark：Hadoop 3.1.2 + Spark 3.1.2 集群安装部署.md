---
layout:     post
title:      Spark：Hadoop 3.1.2 + Spark 3.1.2 集群安装部署
subtitle:
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Spark
---


# 1. 规划

#### 1.1. 版本

- Hadoop 3.1.2
- Spark 3.1.2

#### 1.2. 集群规划

- Master：Hadoop102
- Slave1：Hadoop103
- Slave2：Hadoop104

# 2. 安装软件

- 上传
    - 上传 spark 压缩文件 `spark-3.1.2-bin-hadoop3.2.tgz` 到 `/usr/local`
- 解压缩
    ```
    tar -vxf spark-3.1.2-bin-hadoop3.2.tgz
    mv spark-3.1.2-bin-hadoop3.2 spark
    ```

# 3. 配置环境变量

- 修改 `/etc/profile`
    
    ```
    xsync /etc/profile
    ```
  
    ```
    export SPARK_HOME=/usr/local/spark
    export PATH=$PATH:${SPARK_HOME}/bin:${SPARK_HOME}/sbin
    ```

- 分发文件
    
    ```
    xsync /etc/profile
    ```

- 依次启用生效
    - 在三台机器依次启用环境变量
        ```
        /etc/profile
        ```
    - 注意：此时可能会报错
    
- 报错：`/usr/libexec/grepconf.sh:行5: grep: 未找到命令`
    
    - 问题：
        - 在 `/etc/profile` 配置 SPARK_HOME 后，执行 `source /etc/profile` 环境变量生效，提示报错；
        - 报错后命令行常规命令均无法正常使用
    
    - 经排查发现路径配置错误，需要修改路径，重新启用环境变量配置文件；
    
    - 解决方法：
        - 修改配置文件 SPARK_HOME 路径；
        - cmd 窗口执行如下命令：
            ```
            export PATH=/usr/bin:/usr/sbin:/bin:/sbin:/usr/X11R6/bin
            ```
        - 重新启用环境变量配置文件
            ```
            source /etc/profile
            ```

# 4. 修改 spark 配置

- 修改 `spark-env.sh`
    ```
    vim /spark/conf/spark-env.sh
    ```

    ```
    export JAVA_HOME=/usr/local/bin/jdk1.8
    export SPARK_MASTER_IP=8.142.72.198
    export SPARK_WORKER_MEMORY=1g
    export HADOOP_CONF_DIR=/usr/local/hadoop-3.1.3/etc/hadoop
    ```

- 添加 slave 配置

    ```
    hadoop103
    hadoop104
    ```

# 5. 分发 spark

- 分发 spark 文件夹
    ```
    xsync spark/
    ```

# 6. 启动文件

- start-all.sh
    ```
    cd /usr/local/spark
    ./sbin/start-all.sh
    ```
- 启动成功
    ```
    starting org.apache.spark.deploy.master.Master, logging to /usr/local/spark/logs/spark-root-org.apache.spark.deploy.master.Master-1-hadoop102.out
    hadoop104: starting org.apache.spark.deploy.worker.Worker, logging to /usr/local/spark/logs/spark-root-org.apache.spark.deploy.worker.Worker-1-hadoop104.out
    hadoop103: starting org.apache.spark.deploy.worker.Worker, logging to /usr/local/spark/logs/spark-root-org.apache.spark.deploy.worker.Worker-1-hadoop103.out
    ```

# 7. WEB UI 

- `hadoop102:8080`

    ![]({{site.baseurl}}/img-post/spark-3.png)

