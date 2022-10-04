---
layout:     post
title:      FineBI：Hive SQL 分析某软件聊天数据
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - FineBI
---


# 1. 结果展示

![]({{site.baseurl}}/img-post/finebi-1.png)

# 2. 需求梳理

#### 2.1. 需求指标

- 基于用户聊天数据，统计以下指标信息

    - 统计今日总消息量
    - 统计今日每小时消息量、发送和接收用户数
    - 统计今日各地区发送消息数据量
    - 统计今日发送消息和接收消息的用户数
    - 统计今日发送消息最多的Top10用户
    - 统计今日接收消息最多的Top10用户
    - 统计发送人的手机型号分布情况
    - 统计发送人的设备操作系统分布情况

#### 2.2. 技术方案

- 数据存储：Hadoop Hive
- 分析语言：Hive SQL
- 数据展示：FineBI

# 3. 数据梳理

- 样例数据共两个文件
    - 文件格式 .tsv
    - 制表符诶 tab 键 `\t`
- 每行数据包括以下信息：
    - 消息发送时间
    - 发送人昵称
    - 发送人账号
    - 发送人性别
    - 发送人 ip 地址
    - 发送人操作系统
    - 发送人手机型号
    - 发送人网络类型
    - 发送人的 GPS 定位
    - 接收人昵称
    - 接收人 IP
    - 接收人账号
    - 接收人操作系统
    - 接收人手机型号
    - 接收人网络类型
    - 接收人的 GPS 定位
    - 接收人性别
    - 消息类型
    - 双方距离
    - 消息内容

- 样例数据

```
2021-11-01 07:44:37	郯乐游	1825138366359	男	195.188.222.255	IOS 9.0	OPPO A11X	5G	123.257181,48.807394	梁丘雨琴	136.66.109.160	1523699980735	Android 7.0	华为 荣耀9X	4G	89.332566,42.956064 	女	TEXT	5.14KM	你如那出水的芙蓉，亭亭玉立[lizhigushicom]。你是那样地美，美得让我无法自拔，笑容如春天般清丽秀雅，秀发如柳丝细腻顺滑，眼眸如涟漪百媚丛生。宝贝，对我来说你是最好的。
2021-11-01 07:29:22	牛星海	1551023222496	女	184.147.145.6	Android 7.0	小辣椒 红辣椒8X	4G	113.39623,22.371406	赖美华	186.218.176.9	1593421890833	Android 8.0	小辣椒 红辣椒8X	4G	108.980879,24.071738 	女	TEXT	78.22KM	一点爱一点情，日子甜甜又蜜蜜。一个我一个你，生活平淡不嫌腻。粗茶淡饭，吃出健康身体，简单衣着，也能万种风情。幸福有你，如此心心相印！
2021-11-01 07:57:58	晁泰平	1898238221524	男	21.56.191.12	Android 6.0	一加 OnePlus	4G	117.885171,33.21035	犹妞	225.104.57.224	1331812911843	Android 6	小辣椒 红辣椒8X	5G	104.639117,37.781842 	男	TEXT	98.86KM	一朵花摘了许久枯萎了也舍不得丢，一把伞撑了许久雨停了也记不起收，一条路走了许久天黑了也走不到尽头，一句话想了很久清楚了，才说出口：有你真好！
2021-11-01 07:26:15	茹鸿晖	1328514919221	男	43.227.208.47	Android 8.0	Apple iPhone XR	4G	121.785398,41.86541	肖沛	30.139.30.154	1305190543476	Android 8.0	OPPO Reno3	4G	100.886068,33.395741 	男	TEXT	12.59KM	一见钟情爱上你二话不说想追你三番四次来找你五朵玫瑰送给你六神无主想泡你七次八次来烦你九颗真心打动你十分满意就是你。
2021-11-01 07:01:10	代泰华	1896394119953	男	220.26.70.16	Android 8.0	OPPO Reno3	4G	124.361019,45.856657	邝琨瑶	247.25.68.26	1859080711480	Android 6.0	小辣椒 红辣椒8X	3G	102.725798,37.430552 	男	TEXT	21.2KM	聚散两依依，聚是缘，散也是缘。聚不是开始，散就不是结束。真正的爱情是默契，是心灵的碰撞和颤栗。
```

# 4. 数据存储

#### 4.1. 建库建表

- 建库
    ```
    - 如果数据库已存在就删除
    drop database if exists db_msg cascade ;
    - 创建数据库
    create database db_msg ;
    - 切换数据库
    use db_msg ;
    - 列举数据库
    show databases ;
    ```
- 建表

    ```
    - 如果表已存在就删除
    drop table if exists db_msg.tb_msg_source ;
    - 建表
    create table db_msg.tb_msg_source(
    msg_time string comment "消息发送时间"
    , sender_name string comment "发送人昵称"
    , sender_account string comment "发送人账号"
    , sender_sex string comment "发送人性别"
    , sender_ip string comment "发送人ip地址"
    , sender_os string comment "发送人操作系统"
    , sender_phonetype string comment "发送人手机型号"
    , sender_network string comment "发送人网络类型"
    , sender_gps string comment "发送人的GPS定位"
    , receiver_name string comment "接收人昵称"
    , receiver_ip string comment "接收人IP"
    , receiver_account string comment "接收人账号"
    , receiver_os string comment "接收人操作系统"
    , receiver_phonetype string comment "接收人手机型号"
    , receiver_network string comment "接收人网络类型"
    , receiver_gps string comment "接收人的GPS定位"
    , receiver_sex string comment "接收人性别"
    , msg_type string comment "消息类型"
    , distance string comment "双方距离"
    , message string comment "消息内容" )
    - 指定分隔符为制表符
    row format delimited fields terminated by '\t' ;
    ```
  
#### 4.2. 上传数据

- HDFS上创建目录
    `hdfs dfs -mkdir -p /momo/data`
    
- 上传到HDFS
    - `hdfs dfs -put /home/data1.tsv /momo/data/`
    - `hdfs dfs -put /home/data2.tsv /momo/data/`

- 查询表 验证数据文件是否映射成功
    - `select * from tb_msg_source limit 10;`

- 统计行数
    - `select count(*) as cnt from tb_msg_source;`
    - 返回结果 14 万行；

# 5. ETL 

#### 5.1. 数据探查

- 问题1：当前数据中，有一些数据的字段为空，不是合法数据

    ```
    select
       msg_time,
       sender_name,
       sender_gps
    from db_msg.tb_msg_source
    where length(sender_gps) = 0
    limit 10;
    ```
- 问题2：需求中，需要统计每天、每个小时的消息量，但是数据中没有天和小时字段，只有整体时间字段，不好处理

    ```
    select
       msg_time
    from db_msg.tb_msg_source
    limit 10;
    ```

- 问题3：需求中，需要对经度和维度构建地区的可视化地图，但是数据中GPS经纬度为一个字段，不好处理

    ```
    select
       sender_gps
    from db_msg.tb_msg_source
    limit 10;
    ```
#### 5.2. ETL 过程

- 如果表已存在就删除
    ```
    drop table if exists db_msg.tb_msg_etl;
    ```
- 将Select语句的结果保存到新表中
    ```
    create table db_msg.tb_msg_etl as
    select
      *,
      substr(msg_time,0,10) as dayinfo, - 获取天
      substr(msg_time,12,2) as hourinfo, - 获取小时
      split(sender_gps,",")[0] as sender_lng, - 提取经度
      split(sender_gps,",")[1] as sender_lat - 提取纬度
    from db_msg.tb_msg_source
    ```
- 过滤字段为空的数据
    ```
    where length(sender_gps) > 0 ;
    ```

- 验证ETL结果
    ```
    select
        msg_time,dayinfo,hourinfo,sender_gps,sender_lng,sender_lat
    from db_msg.tb_msg_etl
    limit 10;
    ```

# 6. 指标统计

#### 6.1. 统计今日总消息量
- SQL 
    ```
    create table if not exists tb_rs_total_msg_cnt
    comment "今日消息总量"
    as
    select
      dayinfo,
      count(*) as total_msg_cnt
    from db_msg.tb_msg_etl
    group by dayinfo;
    
    select * from tb_rs_total_msg_cnt;--结果验证
    ```

#### 6.2. 统计今日每小时消息量、发送和接收用户数
- SQL 
    ```
    create table if not exists tb_rs_hour_msg_cnt
    comment "每小时消息量趋势"
    as
    select
      dayinfo,
      hourinfo,
      count(*) as total_msg_cnt,
      count(distinct sender_account) as sender_usr_cnt,
      count(distinct receiver_account) as receiver_usr_cnt
    from db_msg.tb_msg_etl
    group by dayinfo,hourinfo;
    
    select * from tb_rs_hour_msg_cnt;--结果验证
    ```

#### 6.3. 统计今日各地区发送消息数据量
- SQL 
    ```
    create table if not exists tb_rs_loc_cnt
    comment "今日各地区发送消息总量"
    as
    select
      dayinfo,
      sender_gps,
      cast(sender_lng as double) as longitude,
      cast(sender_lat as double) as latitude,
      count(*) as total_msg_cnt
    from db_msg.tb_msg_etl
    group by dayinfo,sender_gps,sender_lng,sender_lat;
    
    select * from tb_rs_loc_cnt; --结果验证
    ```

#### 6.4. 统计今日发送消息和接收消息的用户数
- SQL 
    ```
    create table if not exists tb_rs_usr_cnt
    comment "今日发送消息人数、接受消息人数"
    as
    select
      dayinfo,
      count(distinct sender_account) as sender_usr_cnt,
      count(distinct receiver_account) as receiver_usr_cnt
    from db_msg.tb_msg_etl
    group by dayinfo;
    
    select * from tb_rs_usr_cnt; --结果验证
    ```

#### 6.5. 统计今日发送消息最多的Top10用户
- SQL 
    ```
    create table if not exists tb_rs_susr_top10
    comment "发送消息条数最多的Top10用户"
    as
    select
      dayinfo,
      sender_name as username,
      count(*) as sender_msg_cnt
    from db_msg.tb_msg_etl
    group by dayinfo,sender_name
    order by sender_msg_cnt desc
    limit 10;
    
    select * from tb_rs_susr_top10; --结果验证
    ```

#### 6.6. 统计今日接收消息最多的Top10用户
- SQL 
    ```
    create table if not exists tb_rs_rusr_top10
    comment "接受消息条数最多的Top10用户"
    as
    select
      dayinfo,
      receiver_name as username,
      count(*) as receiver_msg_cnt
    from db_msg.tb_msg_etl
    group by dayinfo,receiver_name
    order by receiver_msg_cnt desc
    limit 10;
    
    select * from tb_rs_rusr_top10;  --结果验证
    ```

#### 6.7. 统计发送人的手机型号分布情况
- SQL 
    ```
    create table if not exists tb_rs_sender_phone
    comment "发送人的手机型号分布"
    as
    select
      dayinfo,
      sender_phonetype,
      count(distinct sender_account) as cnt
    from tb_msg_etl
    group by dayinfo,sender_phonetype;
    
    select * from tb_rs_sender_phone; --结果验证
    ```

#### 6.8. 统计发送人的设备操作系统分布情况
- SQL 
    ```
    create table if not exists tb_rs_sender_os
    comment "发送人的OS分布"
    as
    select
      dayinfo,
      sender_os,
      count(distinct sender_account) as cnt
    from tb_msg_etl
    group by dayinfo,sender_os;
    
    select * from tb_rs_sender_os;  --结果验证
    ```








