---
layout:     post
title:      用户画像：基于拉链表实现 ID Mapping
subtitle:   基于 Hive ETL 实现
date:       2022-01-01
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - 用户画像
---


#### ID-Map

- 用户在未登录 App 的状态下，在 App 站内访问、搜索相关内容时，记录的是设备 id（即cookieid）相关的行为数据。
- 用户在登录 App 后，访问、收藏、下单等相关的行为记录的是账号 id（即userid）相关行为数据。

![]({{site.baseurl}}/img-post/用户标签存储-3.png)

- 虽然是同一个用户，但其在登录和未登录设备时、记录的行为数据之间是未打通的。
- 通过 ID-MApping 打通 userid 和 cookieid 的对应关系，可以在用户登录、未登录设备时都能捕获其行为轨迹。

#### 拉链表

- 缓慢变化维是在维表设计中常见的一种方式，维度并不是不变的，随时间也会发生缓慢变化。
- 如用户的手机号、邮箱等信息可能会随用户的状态变化而改变，再如商品的价格也会随时间变化而调整上架的价格。因此在设计用户、商品等维表时会考虑用缓慢变化维来开发。
- 同样，在设计 ID-Mapping 表时，由于一个用户可以在多个设备上登录，一个设备也能被多个用户登录，所以考虑用缓慢变化维表来记录这种不同时间点的状态变化。

![]({{site.baseurl}}/img-post/用户标签存储-2.png)

-  通过拉链表记录了 userid 每一次关联到不同 cookieid 的情况。
- 其中 start_date 表示该记录的开始日期，end_date 表示该记录的结束日期，当 end_date 为 99991231 时，表示该条记录当前仍然有效。

#### Hive & ETL
    
- 首先，需要从埋点表和访问日志表里面，获取到 cookieid 和 userid 同时出现的访问记录。
    - ods.page_event_log 是埋点日志表；
    - ods.page_view_log 是访问日志表；
    - 将获取到的 userid 和 cookieid 信息插入 cookieid-userid 关系表（ods.cookie_user_signin）中；
    
    ```
    INSERT OVERWRITE TABLE ods.cookie_user_signin PARTITION (data_date = '${data_date}')
      SELECT t.*
        FROM (
             SELECT userid,
                    cookieid,
                    from_unixtime(eventtime,'yyyyMMdd') as signdate
               FROM ods.page_event_log      -- 埋点表
               WHERE data_date = '${data_date}'
            UNION ALL
             SELECT userid,
                    cookieid,
                    from_unixtime(viewtime,'yyyyMMdd') as signdate
               FROM ods.page_view_log   -- 访问日志表
               WHERE data_date = '${data_date}'
               ) t
    ```
        
- 其次，创建 ID-Map 拉链表，将每天新增到 ods.cookie_user_signin 表中的数据与拉链表历史数据做比较，如果有变化或新增数据则进行更新。

    ```
    CREATE TABLE `dw.cookie_user_zippertable`(
    `userid` string COMMENT '账号ID', 
    `cookieid` string COMMENT '设备ID', 
    `start_date` string COMMENT 'start_date', 
    `end_date` string COMMENT 'end_date')
    COMMENT 'id-map拉链表'
    ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t'
    ```

- 创建完成后，每天ETL调度将数据更新到ID-Mapping拉链表中，

    ```
    INSERT OVERWRITE TABLE dw.cookie_user_zippertable
    SELECT t.* 
      FROM (
          SELECT t1.user_num,
                 t1.mobile,
                 t1.reg_date,
                 t1.start_date,
                 CASE WHEN t1.end_date = '99991231' AND t2.userid IS NOT NULL THEN '${data_date}'
                      ELSE t1.end_date
                 END AS end_date
           FROM dw.cookie_user_zippertable t1
        LEFT JOIN (  SELECT *
                     FROM ods.cookie_user_signin
                    WHERE data_date='${data_date}'
                  )t2
               ON t1.userid = t2.userid
    UNION
           SELECT userid,
                  cookieid,
                  '${data_date}' AS start_date,
                  '99991231' AS end_date
            FROM ods.cookie_user_signin
           WHERE data_date = '${data_date
           }'
              ) t
    ```

- 查看某日（如20190801）的快照数据：
    ```
    select  * 
    from dw.cookie_user_zippertable 
    where start_date<='20190801' and end_date>='20190801'
    ```

- 查看某个时间点 userid 对应的 cookieid

    ```
    select cookieid 
    from dw.cookie_user_zippertable 
    where userid='32101029' and start_date<='20190801' and end_date>='20190801'
    ```
    ![]({{site.baseurl}}/img-post/用户标签存储-4.png)
    
    - 上图可看出用户‘32101029’在历史中曾登录过3个设备，通过限定时间段可找到特定时间下用户的登录设备。
    
#### 数据膨胀问题

- 在开发中需要注意关于 userid 与 cookieid 的多对多关联，如果不加条件限制就做关联，很可能引起数据膨胀问题；
- 在实际应用中，会遇到许多需要将 userid 和 cookieid 做关联的情况；
- 例如，需要在 userid 维度开发出该用户近 30 日的购买次数、购买金额、登录时长、登录天数等标签；
- 前两个标签可以很容易地从相应的业务数据表中、根据算法加工出来，而登录时长、登录天数的数据存储在相关日志数据中；
- 日志数据表记录的 userid 与 cookieid 为多对多关系。因此在结合业务需求开发标签时，要确定好标签口径定义。

