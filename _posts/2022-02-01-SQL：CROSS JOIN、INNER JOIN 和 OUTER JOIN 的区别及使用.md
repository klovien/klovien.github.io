---
layout:     post
title:      SQL：CROSS JOIN、INNER JOIN 和 OUTER JOIN 的区别及使用
subtitle:   以员工考勤记录统计为例
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
- 创建日历表 calendar

  ```aidl
  CREATE TABLE calendar(
    id             INTEGER NOT NULL PRIMARY KEY, -- 日历编号
    calendar_date  DATE NOT NULL UNIQUE, -- 日历日期
    calendar_year  INTEGER NOT NULL, -- 日历年
    calendar_month INTEGER NOT NULL, -- 日历月
    calendar_day   INTEGER NOT NULL, -- 日历日
    is_work_day    VARCHAR(1) DEFAULT 'Y' NOT NULL -- 是否工作日
  );
  ```
- 创建考勤记录表 attendance
  ```
  CREATE TABLE attendance(
    id         INTEGER NOT NULL PRIMARY KEY, -- 考勤记录编号
    check_date DATE NOT NULL, -- 考勤日期
    emp_id     INTEGER NOT NULL, -- 员工编号
    clock_in   TIMESTAMP, -- 上班打卡时间
    clock_out  TIMESTAMP, -- 下班打卡时间
    CONSTRAINT uk_attendance UNIQUE (check_date, emp_id)
  );
  ```
- 创建员工表 employee
  ```
  CREATE TABLE `employee` (
    `employee_id` int NOT NULL AUTO_INCREMENT,
    `employee_name` varchar(45) DEFAULT NULL,
    `department_id` int DEFAULT NULL,
    `job_id` int DEFAULT NULL,
    PRIMARY KEY (`employee_id`),
    UNIQUE KEY `employee_id_UNIQUE` (`employee_id`)
  ) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
  ```


# 笛卡尔积（交叉连接）CROSS JOIN 

- 交叉连接不带 WHERE 子句，它返回被连接的两个表所有数据行的笛卡尔积；
- CROSS JOIN 返回结果集合中的数据行数，等于第一个表中的数据行数乘以第二个表中的数据行数；

- 应用实例：
  - 如本文所示，在统计之前，需要将每个员工的 calendar 数据先行 JOIN，然后才能对比出勤情况，这里就会用到 CROSS JOIN；
  ```aidl
  SELECT
    c.*, e.*
  FROM
    calendar c
  CROSS JOIN
    employee e
  LIMIT 1000000;
  ```
  ![sql-9]({{site.baseurl}}/img-post/sql-9.png)

  - 这个时候，CROSS JOIN 其实也可以写成如下样式：
  ```aidl
  SELECT 
      *
  FROM
      calendar,employee
  LIMIT 1000000;
  ```
  ![sql-10]({{site.baseurl}}/img-post/sql-10.png)

# 内连接 INNER JOIN ... ON

- 主要指带 ON 的内连接，内连接按照 ON 条件合并两个表，返回满足条件的记录；
- 应用实例：
  - 在本文示例中，如果我们想查看全部员工的考勤记录，可以使用 INNER JOIN ON，得到全部员工的考勤记录；

    ```aidl
    SELECT
        a.*, e.*
    FROM
        attendance a
    INNER JOIN
        employee e
    ON
        a.emp_id=e.employee_id
    LIMIT 1000000;
    ```
- INNER JOIN ... ON vs 等值连接 WHERE
  - 使用 where 和 = 将表连接起来的查询，其查询结果中列出被连接表中的所有列；
  - 上面的例子，用如下 SQL 也可以获取同样的结果；

    ```aidl
    SELECT
        a.*, e.*
    FROM
        attendance a,employee e
    WHERE
        a.emp_id=e.employee_id
    LIMIT 1000000;
    ```
- 注意：
  - 从逻辑上来说，等值连接与内连接没什么不同；
  - 但实际上等值连接和内连接的执行计划并不相同，**当参与连接的两个表比较大时，使用内连接(inner join)的效率更高**。


# OUTER JOIN

#### LEFT JOIN ON

- 左外连接会保留左表的全部记录，相当于在左表的基础上加上右表中满足 ON 条件的数据；
- 剩余的空位以 NULL 填充；

- 示例：

  ```aidl
  SELECT
      a.*, e.*
  FROM
      employee e
  LEFT JOIN
      attendance a
  ON
      a.emp_id=e.employee_id
  LIMIT 1000000;
  ```
  ![sql-11]({{site.baseurl}}/img-post/sql-11.png)

#### OUTER JOIN

- - 左外连接会保留右表的全部记录，相当于在左表的基础上加上右表中满足 ON 条件的数据；
- 剩余的空位以 NULL 填充；

- 其情况与 LEFT JOIN 刚好相反，具体不再赘述。

