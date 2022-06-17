---
layout:     post  
title:      Tableau： Tableau 基本功能介绍
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Tableau
---




# MySQL 数据库连接

- 下面这种方式，并不能成功下载驱动；

![]({{site.baseurl}}/img-post/tableau-1.jpg)

- 需要自己到 MySQL 网站，自行下载安装；

![]({{site.baseurl}}/img-post/tableau-2.jpg)

- 安装好驱动以后，就可以正常连接 MySQL；

![]({{site.baseurl}}/img-post/tableau-3.jpg)


# 多表连接

#### 跨库取数

>Tableau Prep 不支持跨库直接拖入表，如果直接拖入窗口的话，会提示报错。

- 一般使用自定义 SQL 跨库添加表；

![]({{site.baseurl}}/img-post/tableau-5.jpg)

#### 内联接

使用内联接来合并表时，生成的表将包含与两个表均匹配的值。我们可以理解为两张表的交集。

#### 左联接

使用左联接来合并表时，生成的表将包含左侧表中的所有值以及右侧表中的对应匹配项。

当左侧表中的值在右侧表中没有对应匹配项时，您将在数据网格中看到 null 值。

#### 右联接

使用右联接来合并表时，生成的表将包含右侧表中的所有值以及左侧表中的对应匹配项。

当右侧表中的值在左侧表中没有对应匹配项时，您将在数据网格中看到 null 值。

#### 外联接

使用完全外部联接来合并表时，生成的表将包含两个表中的所有值。

当任一表中的值在另一个表中没有匹配项时，您将在数据网格中看到 null 值。



# 输出结果

#### 输出结果表

- Tableau Prep 只负责 ETL 相关工作，具体的数值计算不在 Tableau Prep 完成。

![]({{site.baseurl}}/img-post/tableau-4.jpg)

#### 发布数据源

![]({{site.baseurl}}/img-post/tableau-map-4.png)






