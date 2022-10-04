---
layout:     post
title:      Hive：HQL 简介及其与 SQL 的区别
subtitle:   
date:       2022-01-26
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Hive
---

# 什么是 HQL

HQL 是 Hibernate Query Language（Hibernate 查询语言）的缩写，提供更加丰富灵活、更为强大的查询能力；HQL 更接近 SQL 语句查询语法。Hibernate 查询语言（HQL）是一种面向对象的查询语言，类似于 SQL，但不是去对表和列进行操作，而是面向对象和它们的属性。 HQL 查询被 Hibernate 翻译为传统的 SQL 查询从而对数据库进行操作。

# 执行 HQL 查询的步骤

  1、获得HibernateSession对象
  2、编写HQL语句
  3、调用Session的createQuery方法创建查询对象
  4、如果HQL语句包含参数，则调用Query的setXxx方法为参数赋值
  5、调用Query对象的list等方法返回查询结果。

二、在Hibernate中推荐使用hql语句，不建议直接使用sql。
原因：
　1、sql不是以面对对象的方式操作数据库，这是Hibernate不提倡的。
　2、如果编写了某种数据库特有的函数或语法，那么在更换数据库时肯定要修改源码重新编译。

三、Hibernate把hql编译成sql语句传送到数据库进行查询。
　在执行效率方面，不考虑其它的影响, 一般sql 效率要高于 hql ,如果考虑 缓存,关联映射,语句的质量就要看具体情况,不过sql 的功能是比hql大。


#### 面向库表 VS 面向对象

- sql 面向数据库表查询；
- hql 面向对象查询。

#### 对象属性做条件 VS 表字段做条件

- hql
    - from 后面跟的 类名＋类对象 where 后 用 对象的属性做条件；
- sql
    - from 后面跟的是表名 where 后 用表中字段做条件查询。

#### 大小写敏感

- 因为 HQL 是面向对象的，而对象类的名称和属性都是大小写敏感的，所以 HQL 是大小写敏感的；
- HQL语句：`from Cat as cat where cat.id > 1;` 与 `from Cat as cat where cat.ID > 1;` 是不一样的，这点与 SQL 不同。






