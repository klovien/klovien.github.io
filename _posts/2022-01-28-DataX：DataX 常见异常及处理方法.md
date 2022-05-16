---
layout:     post
title:      DataX：DataX 常见异常及处理方法
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - DataX
---


# 1. 无法访问 datax-web

- 问题现象：
    - 启动无报错，但无法访问 datax-web 网页

- 处理方法：重新安装初始化 datax

    ```
    cd /home/work/datax-web/build/datax-web-2.1.2/bin & ./install.sh
    ```

# 2. 初始化 datax 时 MySQL 拒绝访问

- 报错内容：

  `ERROR 1045 (28000): Access denied for user 'root'@'xxx.xxx.xxx.xxx' (using password: YES)`

- 处理方法：不要使用 root 账号连接 MySQL，使用 jianzi 账号密码连接 MySQL

    ```
    Please input the db username(default: root): jianzi
    Please input the db password(default: ): 4ylnkL6lRT
    ```

# 3. 启动 DataX 不成功

- 报错内容：
  
  `DATAX-EXEXUTOR didn't start successfully, not found in the java process table`

- 处理方法：
    ```
    ./start-all.sh start -v
    ```

# 4. 无法连接数据源

- 报错内容：
  
  `Code:[DBUtilErrorCode-10], Description:[连接数据库失败. 请检查您的 账号、密码、数据库名称、IP、Port或者向 DBA 寻求帮助(注意网络环境).]. - 具体错误信息为：com.mysql.jdbc.exceptions.jdbc4.MySQLNonTransientConnectionException: Could not create connection to database server.`

- 处理方法：

    - 检查要连接的 MySQL 账号密码；
    - 检查要连接的 MySQL 服务器端口是否开放；
    - 检查要连接的 MySQL 版本，如果是 MySQL8，修改 jdbc 驱动类：
      ```
      com.mysql.cj.jdbc.Driver
      ```

# 5. datax-web 创建任务报错

  - 报错内容：

    `### Error querying database. Cause: com.mysql.jdbc.exceptions.jdbc4.MySQLSyntaxErrorException: Unknown column 't.user_id' in 'field list' ### The error may exist in file [/home/work/datax-web/build/datax-web-2.1.2/modules/datax-admin/conf/mybatis-mapper/JobInfoMapper.xml] ### The error may involve com.wugui.datax.admin.mapper.JobInfoMapper.findAll-Inline ### The error occurred while setting parameters ### SQL: SELECT t.id, t.job_group, t.job_cron, t.job_desc, t.add_time, t.update_time, t.user_id, t.alarm_email, t.executor_route_strategy, t.executor_handler, t.executor_param, t.executor_block_strategy, t.executor_timeout, t.executor_fail_retry_count, t.glue_type, t.glue_source, t.glue_remark, t.glue_updatetime, t.child_jobid, t.trigger_status, t.trigger_last_time, t.trigger_next_time, t.job_json, t.replace_param, t.jvm_param, t.inc_start_time, t.partition_info, t.last_handle_code, t.replace_param_type, t.project_id, t.reader_table, t.primary_key, t.inc_start_id, t.increment_type, t.datasource_id FROM job_info AS t ORDER BY job_desc ASC ### Cause: com.mysql.jdbc.exceptions.jdbc4.MySQLSyntaxErrorException: Unknown column 't.user_id' in 'field list' ; bad SQL grammar []; nested exception is com.mysql.jdbc.exceptions.jdbc4.MySQLSyntaxErrorException: Unknown column 't.user_id' in 'field list'`

    ![]({{site.baseurl}}/img-post/datax-1.png)

    其中的关键错误是：`Unknown column 't.user_id' in 'field list' `。

  - 处理方法：

    连接 dataX 服务器上 MySQL，进入 `dataxweb > jobinfo`，修改 `author(VARCHAR(50))` 字段名为 `user_id(INT(11))`；

    修改后重新创建任务；

    注意：这是个临时解决方案，详细原因并未知晓！！！

# 6. 调度失败

- 报错内容：

    ```
    任务触发类型：手动触发
    调度机器：172.16.0.157
    执行器-注册方式：自动注册
    执行器-地址列表：null
    路由策略：随机
    阻塞处理策略：单机串行
    任务超时时间：0
    失败重试次数：0
    
    >>>>>>>>>>>触发调度<<<<<<<<<<<
    调度失败：执行器地址为空
    ```
- 处理方法：

    - 十分钟后再次执行任务。
    
# 7. `can't find '__main__' module in ''`

- 报错内容：

    ```
    2022-04-20 20:18:57 [JobThread.run-130] <br>----------- datax-web job execute start -----------<br>----------- Param:
    2022-04-20 20:18:57 [BuildCommand.buildDataXParam-100] ------------------Command parameters:
    2022-04-20 20:18:57 [ExecutorJobHandler.execute-57] ------------------DataX process id: -1
    2022-04-20 20:18:57 [ProcessCallbackThread.callbackLog-186] <br>----------- datax-web job callback finish.
    2022-04-20 20:18:57 [AnalysisStatistics.analysisStatisticsLog-53] /usr/bin/python: can't find '__main__' module in ''
    2022-04-20 20:18:57 [JobThread.run-165] <br>----------- datax-web job execute end(finish) -----------<br>----------- ReturnT:ReturnT [code=500, msg=command exit value(1) is failed, content=null]
    2022-04-20 20:18:57 [TriggerCallbackThread.callbackLog-186] <br>----------- datax-web job callback finish.
    ```

- 处理方法：

    - 原因分析：datax-web找不到datax启动文件（datax.py）

    - 修改 datax-executor.sh 文件

        ```
        cd /home/work/datax-web/modules/datax-executor/bin
        vim datax-executor.sh
        ```
      找到下面的代码并注释掉。
        ```
        # JAVA_OPTS=${JAVA_OPTS}" -Dserver.port="${SERVER_PORT}" -Ddata.path="${DATA_PATH}" -Dexecutor.port="${EXECUTOR_PORT}" -Djson.path="${JSON_PATH}" -Dpython.path="${PYTHON_PATH}" -Ddatax.admin.port="${DATAX_ADMIN_PORT}
        ```
      写入成下面的代码。
        ```
        JAVA_OPTS=${JAVA_OPTS}" -Dserver.port="${SERVER_PORT}" -Ddata.path="${DATA_PATH}" -Dexecutor.port="${EXECUTOR_PORT}" -Djson.path="${JSON_PATH}" -Dpython.path="/home/work/datax/bin/datax.py" -Ddatax.admin.port="${DATAX_ADMIN_PORT}
        ```
    - 保存退出，重启 datax-web。
    
# 8. `您提供的配置文件存在错误信息，请检查您的作业配置。`

- 报错内容：
  
    `
    插件[odpsreader,mysqlwriter]加载失败，1s后重试... Exception:Code:[Common-00], Describe:[您提供的配置文件存在错误信息，请检查您的作业配置 .] - 配置信息错误，
    您提供的配置文件[/home/work/datax/plugin/reader/._hbase11xreader/plugin.json]不存在. 请检查您的配置文件.
    经DataX智能分析,该任务最可能的错误原因是:
    com.alibaba.datax.common.exception.DataXException: Code:[Common-00], Description:[您提供的配置文件存在错误信息，请检查您的作业配置。].
     -配置信息错误 ,您提供的配置文件:/home/work/datax/plugin/reader/._hbase11xreader/plugin.json]不存在，请检查您的配置文件。
    `
- 处理方法：

    - 原因分析：操作过 reader 目录的内容，系统会自动生成一个.DS_Store 的系统文件，dataX默认plugin/render都是文件夹，没有做操作系统这种层面的差异处理所以需要手动把错误路径下的._hbase11xreader文件删除即可。
    
    - 处理方法：
    
        ```
        find . -name '._*'|xargs rm -rf
        ```
