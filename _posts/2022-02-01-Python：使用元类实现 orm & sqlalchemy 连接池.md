---
layout:     post
title:      Python：使用元类实现 orm & sqlalchemy 连接池
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. orm
- 什么是 orm
  -  [对象关系映射](http://www.itisedu.com/phrase/200603051342455.html)（[Object Relational Mapping](http://www.itisedu.com/phrase/200604231312415.html)，简称[ORM](http://www.itisedu.com/phrase/200604231312115.html)）是一种为了解决[面向对象](http://www.itisedu.com/phrase/200603101726185.html)与关系[数据库](http://www.itisedu.com/phrase/200602271218062.html)存在的互不匹配的现象的技术，简单的说 ORM 是通过使用描述[对象](http://www.itisedu.com/phrase/200603090845215.html)和数据库之间映射的[元数据](http://www.itisedu.com/phrase/200603141328355.html)，将程序中的对象自动持久化到关系数据库中，本质上就是将数据从一种形式转换到另外一种形式，这也同时暗示者额外的执行开销；
  -  ORM 的全程是对象关系映射，这种思想的要点在于要把后台对数据库的各种操作如增删改查中的一些步骤给抽象出来，把数据表映射成一个类，表中的行给作为一个实例，行中的每个字段作为属性，这样以来以前对数据库插入一行数据的操作、就可以简化为类似下面代码：
```
user=User(id="100001",name="Andy",password="*****")
user.save()  //保存到数据库
```
-  简单理解：**表格就是类，字段名就是属性，一行记录就是一个对象；**
- orm 的优势是什么
  -  ORM将数据与SQL独立开来，让使用时只需要传递数据，调用方法，而不去一遍一遍的拼接sql语句，如果ORM作为一种[中间件](http://www.itisedu.com/phrase/200604241155005.html)实现，则会有很多机会做优化，更重要的是用于控制转换的元数据需要提供和管理，但是同样这些花费要比维护手写的方案要少；而且就算是遵守ODMG规范的对象数据库依然需要[类](http://www.itisedu.com/phrase/200603090857555.html)级别的元数据；
- 为什么需要 orm
  -  ORM，是随着面向对象的[软件开发](http://www.itisedu.com/phrase/200603282233345.html)方法发展而产生的，面向对象的开发方法是当今企业级应用开发环境中的主流开发方法，关系数据库是企业级应用环境中永久存放数据的主流数据存储系统；
  -  对象和关系数据是[业务实体](http://www.itisedu.com/phrase/200603101513085.html)的两种表现形式，业务实体在内存中表现为对象，在数据库中表现为关系数据。内存中的对象之间存在关联和继承关系，而在数据库中，关系数据无法直接表达多对多关联和继承关系，ORM 一般以中间件的形式存在，主要实现程序对象到关系数据库数据的映射；
- 示例：
  -  下面的代码，演示了如何实现 orm：
```

class ModelMetaclass(type):
    """
    元类，被 Model 类继承，提供最基本的
    """
    def __new__(cls, name, bases, attrs):
        """
        :param name: 子类的名称，继承该元类的子类的名称；
        :param bases: 父类，以tuple的形式传入，没有也要传入控tuple：()；
        :param attrs: 绑定的方法或属性，以dict的形式传入；
        :return:
        """
        mappings = dict()
        # 判断是否需要保存
        for key, value in attrs.items():
            # 判断是否是指定的字符串或者数字的实例对象
            if isinstance(value, tuple):
                mappings[key] = value

        for key in mappings.keys():
            attrs.pop(key)

        attrs["__mappings__"] = mappings
        attrs["__table__"] = name.lower()
        return type.__new__(cls, name, bases, attrs)


class Model(object, metaclass=ModelMetaclass):
    """
    Model 类，被 Field 类(如本文中的 User 类)继承
    """
    def __init__(self, **kwargs):
        for name, value in kwargs.items():
            setattr(self, name, value)

    def save(self):
        fileds = []
        args = []
        for key, value in self.__mappings__.items():
            fileds.append(value[0])
            args.append(getattr(self, key, None))

        args_temp = list()
        for temp in args:
            if isinstance(temp, int):
                args_temp.append(str(temp))
            elif isinstance(temp, str):
                args_temp.append(f"""'{temp}'""")
        sql = f"""INSERT INTO {self.__table__} ({','.join(fileds)}) VALUES ({','.join(args_temp)})"""
        print("SQL : ", sql)


class User(Model):
    user_id = ("user_id", "int usigined")                           # 类属性，传入元类 attrs
    user_name = ("username", "varchar(30)")                         # 类属性，传入元类 attrs
    user_email = ("email", "varchar(30)")                           # 类属性，传入元类 attrs


user = User(user_id=123, user_name="pan", user_email="xxx@xx.com")
user.save()

```
- sqlalchemy

![1431912884-5a5b6a87e85dc_articlex.png](https://upload-images.jianshu.io/upload_images/14502986-55e004a93977cc5d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


- 创建 database；
  -  在 MySQL 中创建 databse user ；
```
mysql> create database userdb;
Query OK, 1 row affected (0.00 sec)
```
- 创建 **配置文件 unitConf.conf**：
```

[db]
db_host = localhost
db_port = 3306
db_user = root
db_passwd = Linux423
db_name = userdb

[concurrent]
thread= 10


```
- 使用 sqlalchemy 连接 mysql：
```

from sqlalchemy import exists, Column, Integer, String, ForeignKey, DateTime, Text, func
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.orm import scoped_session
import configparser


cf = configparser.ConfigParser()
cf.read("unitConf.conf")
# 从配置文件中获取数据库信息
host = cf.get("db", "db_host")
port = cf.get("db", "db_port")
dbname = cf.get("db", "db_name")
usernm = cf.get("db", "db_user")
passwd = cf.get("db", "db_passwd")

# 连接数据库
engine_str = "mysql+mysqlconnector://{0}:{1}@{2}:{3}/{4}?auth_plugin=mysql_native_password".format(
                                                                        usernm, passwd, host, port, dbname)
engine = create_engine(
                        engine_str,
                        encoding='utf-8',
                        pool_size=10,               # 连接池中，保持的连接数
                        max_overflow=5              # 当连接数已达到10 且都被使用时,max_overflow就是允许再新建的连接数
)

# 创建DBSession类型
session = sessionmaker(bind=engine)
# declarative_base 函数实例化一个数据库表的基类，之后所有的数据库表都要继承这个基类。
Base = declarative_base()       # 


# 必须继承前面由 declaraive_base 得到的 Base 基类，Base 会通过引擎初始化数据库结构，不继承Base就没有办法连接数据库
class Users(Base):
    # 必须要有__tablename__来指出这个类对应什么表，这个表可以暂时在库中不存在，SQLAlchemy 会帮我们创建这个表
    __tablename__ = "user"

    id = Column(                        # Column 类创建一个字段
        Integer,
        autoincrement=True,
        primary_key=True
    )
    user_no = Column(
        String(20),                     # user_no 员工编号额外生成，作为员工的身份识别编号，每一个编号都是唯一的
        nullable=False,                 # nullable 就是决定是否 not null，
        unique=True,                    # unique 就是决定是否 unique，员工的身份识别编号是唯一的，不能重复
        index=True,                     # 设置 index 可以让系统自动根据这个字段为基础建立索引
        comment='用户编号'
    )
    name = Column(
        String(20),
        nullable=False,                 # 姓名不能为空
        comment='姓名'
    )
    gender = Column(
        Integer,                        # 1 表示男，0 表示 女
        nullable=False,                 # 性别不能为空
        comment='性别'
    )
    age = Column(
        Integer,
        nullable=False,                 # 年龄不能为空
        comment = '年龄'
    )
    dept = Column(
        String(20),
        nullable=False,                 # 部门不能为空
        comment='部门'
    )
    createtime = Column(
        DateTime,
        server_default=func.now(),      # 系统默认时间
        comment='创建时间'
    )
    updatetime = Column(
        DateTime,
        server_default=func.now(),      # 系统默认时间
        onupdate=func.now(),            # onupdate 用于记录【更新时间】
        comment='修改时间'
    )

    def __repr__(self):
        return "<User> {} : {}".format(self.Sname, self.Sno)


def create_table():
    """
    创建表
    :return:
    """
    Base.metadata.create_all(engine)

def delete_table():
    """
    删除表
    """
    Base.metadata.drop_all(engine)

```

- 创建表

-  从上面代码调用 create_table() 方法；
```
create_table()
```
-  查看 mysql 中 user 表结构；
```
mysql> use userdb;
Database changed
mysql> desc user;
+- - - - - - +- - - - - - -+- - - +- - -+- - - - - - - - - -+- - - - - - - - - -+
| Field      | Type        | Null | Key | Default           | Extra             |
+- - - - - - +- - - - - - -+- - - +- - -+- - - - - - - - - -+- - - - - - - - - -+
| id         | int         | NO   | PRI | NULL              | auto_increment    |
| user_no    | varchar(20) | NO   | UNI | NULL              |                   |
| name       | varchar(20) | NO   |     | NULL              |                   |
| gender     | varchar(2)  | NO   |     | NULL              |                   |
| age        | int         | NO   |     | NULL              |                   |
| dept       | varchar(20) | NO   |     | NULL              |                   |
| createtime | datetime    | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
| updatetime | datetime    | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
+- - - - - - +- - - - - - -+- - - +- - -+- - - - - - - - - -+- - - - - - - - - -+
8 rows in set (0.00 sec)
mysql>
```
-  user 表创建成功；

- 删除表

-  从上面代码调用 delete_table() 方法；
```
# 删除表
delete_table()
```
-  在 mysql 中查看 user 表
```
mysql> desc user;
ERROR 1146 (42S02): Table 'userdb.user' doesn't exist
```
-
- pool_size
  设置连接池中，保持的连接数。初始化时，并不产生连接。只有慢慢需要连接时，才会产生连接。例如我们的连接数设置成pool_size=10。如果我们的并发量一直最高是5。那么我们的连接池里的连接数也就是5。当我们有一次并发量达到了10。以后并发量虽然下去了，连接池中也会保持10个连接。

- max_overflow
  当连接池里的连接数已达到，pool_size时，且都被使用时。又要求从连接池里获取连接时，max_overflow就是允许再新建的连接数。
  例如pool_size=10，max_overlfow=5。当我们的并发量达到12时，当第11个并发到来后，就会去再建一个连接，第12个同样。当第11个连接处理完回收后，若没有在等待进程获取连接，这个连接将会被立即释放。

- pool_timeout
  从连接池里获取连接，如果此时无空闲的连接。且连接数已经到达了pool_size+max_overflow。此时获取连接的进程会等待pool_timeout秒。如果超过这个时间，还没有获得将会抛出异常。sqlalchemy默认30秒

- pool_recycle
  这个指，一个数据库连接的生存时间。例如pool_recycle=3600。也就是当这个连接产生1小时后，再获得这个连接时，会丢弃这个连接，重新创建一个新的连接。
  当pool_recycle设置为-1时，也就是连接池不会主动丢弃这个连接。永久可用。但是有可能数据库server设置了连接超时时间。例如mysql，设置的有wait_timeout默认为28800，8小时。当连接空闲8小时时会自动断开。8小时后再用这个连接也会被重置。
