---
layout:     post
title:      DolphinScheduler：DolphinScheduler 使用注意事项
subtitle:   调度方式 & 参数 & 补数据
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Airflow
---



#### DolphinScheduler 调度方式

- 系统支持基于 cron 表达式的定时调度和手动调度。
- 命令类型支持：启动工作流、从当前节点开始执行、恢复被容错的工作流、恢复暂停流程、从失败节点开始执行、补数、定时、重跑、暂停、停止、恢复等待线程。其中 恢复被容错的工作流 和 恢复等待线程 两种命令类型是由调度内部控制使用，外部无法调用。

#### DolphinScheduler 参数关键问题

- 不支持上下游传递
  - 即不支持上游的 out 给下游作为 in 输入
- 不支持在启动流程时候配置参数
- 有全局参数，可以在每个task使用
- OUT 参数只用于存储过程输出

#### DolphinScheduler 补数据（数据回填）

- 示例：基于日期的增量更新

  ```
    select
    字段一,
    字段二,
  
        .....
      
          字段N,
  from 表名
  WHERE DATE_FORMAT(update_time, '%Y-%m-%d')
  >= ${start_time} AND DATE_FORMAT(update_time, '%Y-%m-%d')
  < ${end_time};
  ```
  
- 自定义参数

  ```
  prop: start_time,	value: ${global_start_time}
  prop: end_time,		value: ${global_end_time}
  ```
  
- 正常增量同步时，全局变量赋值

  ```
  prop: global_start_time,	value: date_sub(current_date(),interval 1 day)
  prop: global_end_time,		value: current_date()
  ```

- 补数据时，全局变量赋值

  ```
  prop: global_start_time,	value: 'xxxx-xx-xx'
  prop: global_end_time,		value: 'xxxx-xx-xx'
  ```





