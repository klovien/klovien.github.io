---
layout:     post
title:      Redis：Redis 使用注意事项
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Redis
---




#### 修改 redis.conf 前先做备份
- 修改之前先备份一份默认的初始化配置文件，以备不时之需。


#### Redis 字符串
- 最大字符串为512M，但是生产环境一般不用大字符串。


#### Redis 添加元素
- 江湖规矩一般从左端Push，右端Pop，即LPush/RPop。


#### set 无序集合
- set 无序集合，主要应用于 ```去重、过滤```；
- SPOP：
  -- spop 

#### 从集合 ```随机移除一个成员，并将其返回```；
- SREM：
  -- srem

#### 在name对应的集合中 ```删除某些值```；
- SCARD：
  -- scard 获取name对应的集合中 ```元素个数```；
- SDIFF：
  -- sdiff 在第一个name对应的集合中且不在其他name对应的集合的元素集合
- SDIFFSTORE：
  -- sdiffstore　  

#### 获取第一个name对应的集合中且不在其他name对应的集合，再将其新加入到dest对应的集合中
- SINTER：
  -- sinter 

#### 获取多个name对应集合的并集
- SINTERSTORE：
  -- sinterstore 

#### 获取多一个name对应集合的并集，再讲其加入到dest对应的集合中
- SISMEMBER：
  -- sismember 

#### 检查value是否是name对应的集合的成员
- SMEMBERS：
  -- smembers 

#### 获取name对应的集合的所有成员
- SMOVE：
  -- smove 

#### 将某个成员从一个集合中移动到另外一个集合
- SRANDMEMBER：
  --srandmember

#### 从name对应的集合中 ```随机获取 numbers 个元素```，不会删除
- SUNIONSTORE：
  --sunionstore

#### 获取多一个name对应的集合的并集，并将结果保存到dest对应的集合中
- SSCAN：
  --sscan

#### 同字符串的操作，用于 ```增量迭代分批获取元素```，避免内存消耗太大


#### zset 有序集合
- zset 有序集合，主要应用于 ```排名、排序```；


#### Redis 设置密码
- 在 redis 文件夹打开 ```conf 配置文件``` ，查找 ```requirepass foobared```，找到后将这一行注释掉，在下方添加 ```requirepass XXX（你要设置的密码）```；
- 重启 redis 密码即可生效；


#### Redis 服务化运行
- 以 Windows 为例，cmd 命令行进入 redis 文件夹，执行下面的命令即可安装 redis 服务：
```
redis-server --service-install redis.windows-service.conf --loglevel verbose
```
- 启动 redis 服务
```
redis-server --service-start
```
- 关闭 redis 服务
```
redis-server --service-stop
```


#### Redis 设置远程连接
在 Redis 文件夹打开 ```.conf``` 配置文件；
查找 ```bind 127.0.0.1```，找到后将这一行注释掉，在下方添加 ```bind 0.0.0.0```；
查找 ```protected-mode yes```，找到活将这一行注释掉，在下方添加 ```protected-mode no```；
重启 redis 即开启远程访问；


#### redis 数据持久化策略
- RDB
-


#### redis 内存空间设置
- 进入 redis 文件夹，打开 ```.conf``` 配置文件；
- 查找 ```maxmemory```，找到后修改 ```maxmemory``` 后面的数值，例如：```2000mb``` 表示 ```2G```；


#### redis 最大连接数


#### redis key 超时设置
- 当client主动访问key会先对key进行超时判断，过时的key会立刻删除。如果clien永远都不再get那条key呢？ 它会在Master的后台，每秒10次的执行如下操作： 随机选取100个key校验是否过期，如果有25个以上的key过期了，立刻额外随机选取下100个key(不计算在10次之内)。可见，如果过期的key不多，它最多每秒回收200条左右，如果有超过25%的key过期了，它就会做得更多，但只要key不被主动get，它占用的内存什么时候最终被清理掉只有天知道。在主从复制环境中，由于上述原因存在已经过期但是没有删除的key，在主snapshot时并不包含这些key，因此在slave环境中我们往往看到dbsize较master是更小的。


#### redis 出现异常时的持久化配置
- 默认情况下，redis 出现异常时就会中断持久化写入操作，```.conf``` 配置文件中配置项 ```stop-writes-on-bgsave-error``` 默认设置为 ```yes```，表示 ```redis 遇到错误时终止写入```；
- 这种设置下，当 redis 出现异常的时候，就会出现如下报错：
```json
(error) MISCONF Redis is configured to save RDB snapshots, but is currently not able to persist on
 disk. Commands that may modify the data set are disabled. Please check Redis logs for details 
about the error.
```
- 解决这个报错有两种思路：
  -- 第一种是 ```回溯分析 redis 到底出现了什么问题，从而在应用层面予以解决```，但这会影响应用的执行，如果问题没有查找清楚这个报错就会一直阻断程序运行；
  -- 第二种是 打开 ```conf 配置文件```，修改  ```stop-writes-on-bgsave-error``` 为 ```no```，之后重启 redis，这可以看做是一种临时解决方案，好处是不会影响程序的执行；
- 最好是在配置 redis  的时候，就在 conf 配置文件中把  ```stop-writes-on-bgsave-error``` 的值修改为 ```no```，毕竟应用程序中断是最不愿意看到的情况。
