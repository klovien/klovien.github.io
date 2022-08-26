---
layout:     post
title:      Python：Python SQLalchemy 使用
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. sqlalchemy 中 create_engine 中的参数
- pool_size
  - 设置连接池中，保持的连接数；
  - 初始化时并不产生连接，只有慢慢需要连接时，才会产生连接；
  - 例如我们的连接数设置成 pool_size=10，如果我们的并发量一直最高是 5，那么我们的连接池里的连接数也就是5，当我们有一次并发量达到了 10，以后并发量虽然下去了，连接池中也会保持 10 个连接。

- max_overflow
  - 当连接池里的连接数已达到 pool_size 且都被使用时，max_overflow就是允许再新建的连接数；
  - 例如pool_size=10，max_overlfow=5，当我们的并发量达到 12 时，当第 11 个并发到来后，就会去再建一个连接，第 12 个同样，当第11个连接处理完回收后，若没有在等待进程获取连接，这个连接将会被立即释放。

- pool_timeout
  - 从连接池里获取连接，如果此时无空闲的连接，且连接数已经到达了 pool_size+max_overflow，此时获取连接的进程会等待pool_timeout秒，如果超过这个时间，还没有获得将会抛出异常；
  - sqlalchemy 默认 30 秒；

- pool_recycle
  - pool_recycle 指一个数据库连接的生存时间；
  - 例如 pool_recycle=3600，也就是当这个连接产生 1 小时后，再获得这个连接时，会丢弃这个连接，重新创建一个新的连接；
  - 当 pool_recycle 设置为 -1 时，也就是连接池不会主动丢弃这个连接、永久可用，但是有可能数据库 server 设置了连接超时时间，例如 mysql 设置的有 wait_timeout 默认为 28800、即 8 小时，当连接空闲 8 小时时会自动断开，8 小时后再用这个连接也会被重置。

# 2. 连接实例
- sqlalchemy + mysql 应用示例：
```
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
from sqlalchemy.orm import scoped_session
from models import Student,Course,Student2Course
from threading import Thread


engine = create_engine(
        "mysql+pymysql://root:123456@127.0.0.1:3306/s9day120?charset=utf8",
        max_overflow=0,  # 超过连接池大小外最多创建的连接
        pool_size=5,  # 连接池大小
        pool_timeout=30,  # 池中没有线程最多等待的时间，否则报错
        pool_recycle=-1  # 多久之后对线程池中的线程进行一次连接的回收（重置）
    )
SessionFactory = sessionmaker(bind=engine)
session = scoped_session(SessionFactory)


def task():
    """"""
    # 方式一：
    """
    # 查询
    # cursor = session.execute('select * from users')
    # result = cursor.fetchall()

    # 添加
    cursor = session.execute('INSERT INTO users(name) VALUES(:value)', params={"value": 'wupeiqi'})
    session.commit()
    print(cursor.lastrowid)
    """
    # 方式二：
    """
    # conn = engine.raw_connection()
    # cursor = conn.cursor()
    # cursor.execute(
    #     "select * from t1"
    # )
    # result = cursor.fetchall()
    # cursor.close()
    # conn.close()
    """

    # 将连接交还给连接池
    session.remove()


for i in range(20):
    t = Thread(target=task)
    t.start()
```