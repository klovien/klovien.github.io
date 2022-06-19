---
layout:     post
title:      SQL：使用 CASE 和 GROUP BY 实现数据透视
subtitle:   统计不同产品、不同
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - SQL
---

# 创建表
- 需求：
  - 需要根据 employee、attendance、calendar 三张表，统计员工的出勤情况；
- - 创建销售数据表sales_data
```
-- saledate表示销售日期，product表示产品名称，channel表示销售渠道，amount表示销售金额

CREATE TABLE sales_data(saledate DATE, product VARCHAR(20), channel VARCHAR(20), amount NUMERIC(10, 2));
```

# 写入测试数据



# SQL 


```
- 得到结果

    ![sql-10]({{site.baseurl}}/img-post/sql-10.png)
