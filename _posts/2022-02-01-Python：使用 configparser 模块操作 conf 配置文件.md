---
layout:     post
title:      Python：使用 configparser 模块操作 conf 配置文件
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. configparser

- configparser 模块在 python 中是用来读取配置文件；
- configparser 配置文件可以包含一个或多个节（section），每个节可以有多个参数（键=值）；
- configparser 配置文件通常以 .conf 为后缀，作为单独文件进行保存；

# 2. 示例

- 下面为 unitConf.conf 配置文件示例：

```

[db]
db_host = localhost
db_port = 3306
db_user = xxx
db_passwd = xxxxxx
db_name = xxx

[concurrent]
thread= 10

```
- 下面为 .py 程序获取配置的示例：
```
import configparser


cf = configparser.ConfigParser()
cf.read("unitConf.conf")

# return all section
secs = cf.sections()
print('sections:', secs)   # sections: ['db', 'concurrent']

opts = cf.options("db")
print('options:', opts)    # options: ['db_host', 'db_port', 'db_user', 'db_passwd']

kvs = cf.items("db")
print('db:', kvs)          # db: [('db_host', 'localhost'), ('db_port', '3306'), ('db_user', 'xxx'), ('db_passwd', 'xxxxxx')]

# read by type
host = cf.get("db", "db_host")
port = cf.get("db", "db_port")
dbname = cf.get("db", "db_name")
usernm = cf.get("db", "db_user")
passwd = cf.get("db", "db_passwd")

print(host)                # localhost
print(port)                # 3306
print(dbname)              # xxx
print(usernm)              # xxx
print(passwd)              # xxxxxx

```