---
layout:     post
title:      Hive：Hive SQL DDL 基本操作
subtitle:   
date:       2022-01-26
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hive
---

# 1. Hive 建库

- 创建 database
    ```
    create database test;
    ```
- 查看库
    - 文件位置：`/user/hive/warehouse`
    - 看到 test.db；
    ![]({{site.baseurl}}/img-post/hive-8.png)

# 2. Hive 表 创建 & 删除

#### 2.1. 建表

- 创建表 t_archer
    ```
    create table t_archer(
    id int comment "ID",
    name string comment "英雄名称",
    hp_max int comment "最大生命",
    mp_max int comment "最大法力",
    attack_max int comment "最高物攻",
    defense_max int comment "最大物防",
    attack_range string comment "攻击范围",
    role_main string comment "主要定位",
    role_assist string comment "次要定位"
    ) comment "王者荣耀射手信息"
    row format delimited
    fields terminated by "\t"; -- 字段之间的分隔符是 tab 键
    ```
  
- 创建成功
    ![]({{site.baseurl}}/img-post/hive-9.png)

- 上传文件至 master 服务器
    - <a href="{{site.baseurl}}/files/archer.txt">点击下载站内资源：archer.txt</a>

- put 文件至 hdfs 
    ```
    hadoop fs -put ./archer.txt /user/hive/warehouse/test.db/t_archer
    ```

- 查看文件
    ![]({{site.baseurl}}/img-post/hive-10.png)

#### 2.2. 删除表

- 删除表 t_archer 
    ```
    drop table t_archer;
    ```

# 3. 默认分隔符使用

#### 3.1. Linux 文件分隔符
     
- 以 \001 作为分隔符时，下载后用 notePad++ 打开时看到的 SOH
- 以 \002 作为分隔符时，下载后用notePad++打开时看到的 STX，
- 以 \003 作为分隔符时，下载后用notePad++打开时看到的 ETX；
- "\001" “\002” "\003"分隔是程序代码中进行解析需要的。

#### 3.2. Hive 默认使用 \001 作为分隔符

- 使用 notepad++ 查看 \001 分隔符显示为 SOH
    ![]({{site.baseurl}}/img-post/hive-13.png)

#### 3.3. 替换分隔符方法

- notepad++ 替换
    ![]({{site.baseurl}}/img-post/hive-12.png)

#### 3.4. 使用默认分隔符建表

- 创建表 t_team_ace_player
    ```
    create table t_team_ace_player (
    id int,
    team_name string,
    ace_player_name string
    );
    ```

- 上传文件至 master 服务器
    - <a href="{{site.baseurl}}/files/team_ace_player.txt">点击下载站内资源：team_ace_player.txt</a>

- put 文件至 hdfs 
    ```
    hadoop fs -put ./archer.txt /user/hive/warehouse/test.db/team_ace_player
    ```

- 查看文件
    ![]({{site.baseurl}}/img-post/hive-14.png)


# 4. Hive 表查询

- select 查询全表
    ```
    select * from t_archer;
    ```
- 返回结果
    ![]({{site.baseurl}}/img-post/hive-11.png)

# 5. Hive show 语句

- 查库
    ```
    show database;
    ```
- 查表
    ```
    show tables in test;
    ```
- 查看表结构
    ```
    use test;
    desc formatted t_team_ace_player;
    ```

    ![]({{site.baseurl}}/img-post/hive-15.png)

# 6. 中文注释乱码问题解决

#### 6.1. MySQL 的 Hive 元数据存储格式不支持中文

- Hive 元数据是在 MySQL 中保存，存储在 MySQL 中的 Hive 元数据存储格式不支持中文；
- 我们查看表结构元数据信息，发现中文注释都变成了乱码；
    ![]({{site.baseurl}}/img-post/hive-16.png)

#### 6.2. 修改 MySQL 中的 hive 元数据存储格式

- 连接 MySQL 做出如下修改

    ```
    --注意 下面sql语句是需要在MySQL中执行  修改Hive存储的元数据信息（metadata）
    use hive3;
    show tables;
    
    alter table hive3.COLUMNS_V2 modify column COMMENT varchar(256) character set utf8;
    alter table hive3.TABLE_PARAMS modify column PARAM_VALUE varchar(4000) character set utf8;
    alter table hive3.PARTITION_PARAMS modify column PARAM_VALUE varchar(4000) character set utf8 ;
    alter table hive3.PARTITION_KEYS modify column PKEY_COMMENT varchar(4000) character set utf8;
    alter table hive3.INDEX_PARAMS modify column PARAM_VALUE varchar(4000) character set utf8;
    ```

#### 6.3. 删除原来的表重新创建

- 删除 t_archer 后，重新建表；
- 查看表注释显示为中文；
    ![]({{site.baseurl}}/img-post/hive-17.png)

