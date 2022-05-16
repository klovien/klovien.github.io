---
layout:     post
title:      DataX：DataX 数据同步工具部署使用方法
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - DataX
---

> 关于 DataX 的介绍，请参考 <a href="https://dex0423.github.io/2022/01/27/ETL-ETL%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%A6%82%E5%BF%B5/">《ETL：ETL的基本概念》</a> 中 ***5.2. DataX*** 一节的内容。

# 1. 环境准备

##### 1.1. python

- 网上文章推荐 python 2.6，但实际 python 3.7 一样支持，自行根据需要选择；
- 注意安装好以后配置环境变量； 
- 安装配置细节，此处不再赘述；

##### 1.2. JDK 

- JDK 需要 1.6 以上，推荐使用 1.6；
- 细节此处不再赘述；

##### 1.3. Apache Maven 

- 安装 Apache Maven 3.x (Compile DataX)；
- 细节此处不再赘述；

# 2. DataX 安装部署

##### 2.1. 安装步骤

- 环境准备

    - python

      服务器自带 Python2 和 Python3，不需要重新安装；

    - JAVA_HOME

        ```
        sudo apt install -y default-jdk
        ```

- 下载安装
    - 在 /home 下新建并进入 work 目录
        ```
        sudo mkdir /home/work & cd /home/work
        ```

    - 下载解压 datax
        ```
        wget http://datax-opensource.oss-cn-hangzhou.aliyuncs.com/datax.tar.gz
        tar -zxvf datax.tar.gz
        
        ```
    - 下载解压 data-web
        - 百度网盘：https://pan.baidu.com/s/13yoqhGpD00I82K4lOYtQhg，密码：cpsk；
        - 手动上传到 /home/work 目录中，并解压缩；
            ```
            tar -zxvf datax-web-2.1.2.tar.gz
            ```
- 安装 & 初始化

    ```
    cd /home/work/datax-web-2.1.2/bin
    ./install.sh
    
    ...
    Do you want to initalize database with sql: [/home/work/datax-web/build/datax-web-2.1.2/bin/db/datax_web.sql]? (Y/N)y
    Please input the db host(default: 127.0.0.1): 
    Please input the db port(default: 3306): 
    Please input the db username(default: root): jianzi
    Please input the db password(default: ): 4ylnkL6lRT
    Please input the db name(default: dataxweb)
    ...
  ```

##### 2.2. 修改 DB 配置

- 修改 datax 服务器本地 MySQL 数据库，用于存储 datax 账号密码、同步任务、数据源连接信息等；

    ```
    vim /home/work/datax-web/build/datax-web-2.1.2/modules/datax-admin/conf/bootstrap.properties
    
    #Database
    DB_HOST=xxx.xxx.xxx.xxx
    DB_PORT=3306
    DB_USERNAME=xxx
    DB_PASSWORD=xxx
    DB_DATABASE=dataxweb
    ```

##### 2.3. 初始化

- 初始化操作时，会重新在 MySQL 中新建用户和任务信息，如果是重新初始化，记得将 MySQL dataxweb 库中的数据做备份。

    ```
    cd /home/work/datax-web/build/datax-web-2.1.2/bin & ./install.sh
    
    ...
    Do you want to initalize database with sql: [/home/work/datax-web/build/datax-web-2.1.2/bin/db/datax_web.sql]? (Y/N)y
    Please input the db host(default: 127.0.0.1): 
    Please input the db port(default: 3306): 
    Please input the db username(default: root): xxxx
    Please input the db password(default: ): xxxx
    Please input the db name(default: dataxweb)
    ...
    ```

##### 2.4. 添加 MySQL8 需要的 jar 包

- mysql-connector-java-8.0.28.jar 文件下载地址
  
    - <a href="{{site.baseurl}}/files/mysql-connector-java-8.0.28.jar">点击下载站内资源：mysql-connector-java-8.0.28.jar</a>

- 查找文件 mysql-connector-java-5.1.47.jar 所在位置
    ```
    find -name "*mysql-con*"
    ```

- 下面返回的是文件路径示例，这些目录全部都要进行添加
    ```
    /home/work/datax-web-2.1.2/modules/datax-admin/lib/mysql-connector-java-5.1.47.jar
    /home/work/datax/plugin/writer/drdswriter/libs/mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/writer/drdswriter/libs/._mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/writer/mysqlwriter/libs/mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/writer/mysqlwriter/libs/._mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/writer/adswriter/libs/._mysql-connector-java-5.1.31.jar
    /home/work/datax/plugin/writer/adswriter/libs/mysql-connector-java-5.1.31.jar
    /home/work/datax/plugin/reader/drdsreader/libs/mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/reader/drdsreader/libs/._mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/reader/oceanbasev10reader/libs/mysql-connector-java-5.1.40.jar
    /home/work/datax/plugin/reader/mysqlreader/libs/mysql-connector-java-5.1.34.jar
    /home/work/datax/plugin/reader/mysqlreader/libs/._mysql-connector-java-5.1.34.jar
    ```
- 将 mysql-connector-java-8.0.28.jar 文件放到上面返回的文件所在目录


# 3. DataX 使用

##### 3.1. DataX 启动命令

- 在启动前，记得先执行关闭操作；
  
    ```
    cd /home/work/datax-web/build/datax-web-2.1.2/bin
    bash stop-all.sh
    ./start-all.sh
    ```

##### 3.2. 访问 datax-web

- 访问地址：http://xxx.xxx.xxx.xxx:9527/index.html

- 访问datax-web 记住务必加/index.html，不加会提示 `Whitelabel Error Page` 报错！

##### 3.3. DataX 用户管理

- 注意：首次启动，一定要修改 admin 密码！

    修改路径： `用户管理 > admin > 编辑 > 密码`


##### 3.4. MySQL8 修改 jdbc 驱动类

- EDS 如果连接 MySQL8，数据源连接需要修改 jdbc 驱动类

  jdbc 驱动类：
    
    ```
    com.mysql.cj.jdbc.Driver
    ```

# 4. DataX 任务管理

##### 4.1. DataX 任务模板

- 阻塞处理
  
    - 选择：**单机串行**；

        ![]({{site.baseurl}}/img-post/datax-2.png)
  

##### 4.2. 创建任务：reader 增量同步

- 增量同步 where 条件：

  更新指定时间内的数据。

  ```
  "where": "last_update_time>=subdate(current_date, 1)",    # 更新昨天的数据
  
  "where": "last_update_time>=subdate(current_date, 7)",    # 更新七天内的数据
  ```

#####  4.3. 创建任务：writer 替换更新

- 写入模式使用 replace；
    
  replace 在写入数据的时候，会对主键相同的数据进行 update，而不是 insert。
  
  ```
  "writeMode": "replace",
  ```
  
##### 4.4. 切分字段

- 如果要同步的数据量比较大，需要在创建任务的时候，添加主键作为切分字段。
  
  ```
  "splitPk": "sales_order_id"
  ```

##### 4.5. 任务示例

```
{
  "job": {
    "setting": {
      "speed": {
        "channel": 3,
        "byte": 1048576
      },
      "errorLimit": {
        "record": 0,
        "percentage": 0.02
      }
    },
    "content": [
      {
        "reader": {
          "name": "mysqlreader",
          "parameter": {
            "where": "last_update_time>=subdate(current_date, 7)",
            "username": "xxxxx",
            "password": "xxxxx",
            "column": [
              "`add_time`",
              "`last_update_time`",
              "`日期`",
              "`店铺_系统或分销商`",
              "`规格编码`",
              "`发货数量`",
              "`退货数量`",
              "`销售数量`",
              "`发货金额`",
              "`退货金额`",
              "`实际销售额`",
              "`数据来源表`",
              "`发退货类型`"
            ],
            "splitPk": "xxx_id",
            "connection": [
              {
                "table": [
                  "xxxxxx"
                ],
                "jdbcUrl": [
                  "jdbc:mysql://xxx.xxx.xxx.xxx:3306/xxx"
                ]
              }
            ]
          }
        },
        "writer": {
          "name": "mysqlwriter",
          "parameter": {
            "writeMode": "replace",
            "username": "xxxxx",
            "password": "xxxxx",
            "column": [
              "`source_add_time`",
              "`source_last_update_time`",
              "`日期`",
              "`店铺_系统或分销商`",
              "`规格编码`",
              "`发货数量`",
              "`退货数量`",
              "`销售数量`",
              "`发货金额`",
              "`退货金额`",
              "`实际销售额`",
              "`数据来源表`",
              "`发退货类型`"
            ],
            "connection": [
              {
                "table": [
                  "ods_xxxxxx"
                ],
                "jdbcUrl": "jdbc:mysql://xxx.xxx.xxx.xxx:3306/ods"
              }
            ]
          }
        }
      }
    ]
  }
}
```
