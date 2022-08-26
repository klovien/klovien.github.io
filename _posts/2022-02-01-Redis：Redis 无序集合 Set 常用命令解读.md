---
layout:     post
title:      Redis：Redis 无序集合 Set 常用命令解读
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Redis
---

#### 前言

在Redis中，集合（Set）是一个无序的字符串数据集，且该数据集中的元素具有唯一性（即不存在重复的元素）。

在下面的文章中，列举了日常工作中我们常用的 Set 集合命令：

#### SADD

`SADD`命令用于将指定元素添加到集合中，并返回实际添加的元素个数（即不包括已经存在的元素个数）。当指定的元素在集合中已经存在时，将忽略该元素。若指定的键不存在，在执行操作前将创建一个新的空集合。

```
SADD key member [member ...]

```

在Redis 2.4及以上版本中，`SADD`命令可用于一次添加多个元素。

###### 示例

```
redis> SADD numbers 1
(integer) 1
# 元素1已存在，第二次执行时将被忽略
redis> SADD numbers 1 2 3
(integer) 2
redis> SMEMBERS numbers
1) "1"
2) "2"
3) "3"

```

#### SISMEMBER

`SISMEMBER`命令用于指示集合中是否存在指定元素，若存在则返回`1`，否则返回`0`（若集合不存在则视为空集合，也将返回`0`）。

```
SISMEMBER key member

```

###### 示例

```
redis> SADD numbers 1
(integer) 1
redis> SISMEMBER numbers 1
(integer) 1
redis> SISMEMBER numbers 2
(integer) 0

```

#### SREM

`SREM`命令用于移除集合中的指定元素，并返回实际移除的元素个数。当集合中不存在指定的元素时将忽略对应的移除操作；若集合不存在，则将视为空集合，并返回`0`。

```
SREM key member [member ...]

```

在Redis 2.4及以上版本中，`SREM`命令可用于一次移除多个元素。

###### 示例

```
redis> SADD numbers 1 2 3 4
(integer) 4
redis> SREM numbers 3 4 5
(integer) 2
redis> SMEMBERS numbers
1) "1"
2) "2"

```

#### SCARD

`SCARD`命令用于返回集合的基数（cardinality，即集合中元素的个数），若集合不存在则将返回`0`。

```
SCARD key

```

###### 示例

```
redis> SADD numbers 1 2 3
(integer) 3
redis> SADD numbers 4
(integer) 1
redis> SCARD numbers
(integer) 4

```

#### SMEMBERS

`SMEMBERS`命令用于已数组的形式返回集合中的所有元素，若集合不能存在则视为空集合。

```
SMEMBERS key

```

###### 示例

```
# 键numbers不存在
redis> SMEMBERS numbers
(empty array)
redis> SADD numbers 1 2 3
(integer) 3
redis> SMEMBERS numbers
1) "1"
2) "2"
3) "3"

```

#### SSCAN

`SSCAN`命令与前文中介绍过的[`SCAN`](https://www.ghosind.com/2020/06/30/redis-keys#scan)命令类似，它用于增量式的迭代获取集合中的所有元素。同样，`SSCAN`命令是一个基于游标`cursor`的迭代器，每次执行后将会返回一个新的游标，以作为下一轮迭代的游标参数。关于更多`SSCAN`命令的用法，可参考[`SCAN`](https://www.ghosind.com/2020/06/30/redis-keys#scan)命令。

```
SSCAN key cursor [MATCH pattern] [COUNT count]

```

###### 示例

```
redis> SSCAN numbers
1) "0"
2) (empty array)
redis> SADD numbers 1 2 3
(integer) 3
redis> SSCAN numbers 0
1) "0"
2) 1) "1"
   2) "2"
   3) "3"

```

#### SRANDMEMBER

`SRANDMEMBERS`命令用于随机获取元素，并在Redis 2.6及以上版本中支持通过`count`参数指定获取的元素个数。

```
SRANDMEMBER key [count]

```

对于`count`参数，不同的值将有以下几种情况：

*   若其值为正数则随机返回集合中指定数量的元素，且所有元素不重复；
*   若其值大于集合的大小则返回集合中的全部元素；
*   若其值为负数，则将随机获取该值绝对值数量的元素，且可能存在重复的元素；
*   若其值为0，则返回空数组。

###### 返回值

当未指定`count`参数时：

*   随机返回集合内的一个元素；
*   若集合不存在则返回`nil`。

若通过`count`参数指定获取的元素个数时：

*   返回随机获取的元素数组；
*   若集合不存在则返回空数组。

###### 示例

```
redis> SADD numbers 1 2 3 4 5
(integer) 5
redis> SRANDMEMBER numbers
"4"
redis> SRANDMEMBER numbers 2
1) "2"
2) "3"

```

#### SPOP

`SPOP`命令用于随机地从集合中移除并返回元素，若集合不存在则返回`nil`。在Redis 3.2及以上的版本中，`SPOP`命令支持通过`count`参数指定获取的元素个数。若`count`的值大于集合的大小，将移除并返回集合中的全部元素。

```
SPOP key [count]
```

###### 示例

```
127.0.0.1:6379> SADD numbers 1 2 3 4 5
(integer) 5
127.0.0.1:6379> SPOP numbers
"3"
127.0.0.1:6379> SPOP numbers
"2"
127.0.0.1:6379> SPOP numbers
"5"
127.0.0.1:6379> SPOP numbers
"4"
127.0.0.1:6379> SPOP numbers
"1"
127.0.0.1:6379> SPOP numbers
(nil)
127.0.0.1:6379> SCARD numbers
(integer) 0
127.0.0.1:6379>

```

#### SMOVE

`SMOVE`命令用于将源集合中的指定元素移至目标集合中，即将该元素从源集合中移除并在目标集合中添加，并返回`1`。当源集合中不存在指定的元素时，将不执行操作并返回`0`。

```
SMOVE source destination member

```

`SMOVE`命令具备原子性，即在执行时其它客户端的连接只会在源集合或目标集合中获取到该元素。

###### 示例

```
redis> SADD set1 1 2
(integer) 2
redis> SADD set2 3
(integer) 1
redis> SMOVE set1 set2 2
(integer) 1
redis> SMEMBERS set1
1) "1"
redis> SMEMBERS set2
1) "2"
2) "3"

```

#### SINTER

`SINTER`命令用于获取指定集合的交集。当集合不存在将被视为空集合，若参数中包含空集合，返回的结果也为空（任何集合与空集合的交集为空集）。

当只指定一个集合作为参数时，执行该命令等同于执行`SMEMBERS`命令。

```
SINTER key [key ...]

```

官方文档中给出了下列示例：

```
key1 = {a,b,c,d}
key2 = {c}
key3 = {a,c,e}
SINTER key1 key2 key3 = {c}

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SINTER set1 set2
1) "3"

```

#### SINTERSTORE

`SINTERSTORE`命令与`SINTER`命令相似，区别在于`SINTERSTORE`命令不直接返回其交集，而是保存到`destination`参数指定的集合中，并返回结果的数量。若`destination`参数指定的集合已存在将会被覆盖。

```
SINTERSTORE destination key [key ...]

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SINTERSTORE set3 set1 set2
(integer) 1
redis> SMEMBERS set3
1) "3"

```

#### SUNION

`SUNION`命令用于获取指定集合的并集。若集合不存在将被视为空集合。

```
SUNION key [key ...]

```

官方文档中给出了下列示例：

```
key1 = {a,b,c,d}
key2 = {c}
key3 = {a,c,e}
SUNION key1 key2 key3 = {a,b,c,d,e}

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SUNION set1 set2
1) "1"
2) "2"
3) "3"
4) "4"
5) "5"

```

#### SUNIONSTORE

`SUNIONSTORE`命令与`SUNION`命令相似，区别在于`SUNIONSTORE`命令不直接返回其并集，而是保存到`destination`参数指定的集合中，并返回结果的数量。若`destination`参数指定的集合已存在将会被覆盖。

```
SUNION key [key ...]

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SUNIONSTORE set3 set1 set2
(integer) 5
redis> SMEMBERS set3
1) "1"
2) "2"
3) "3"
4) "4"
5) "5"

```

#### SDIFF

`SDIFF`命令用于获取指定集合（即第一个参数指定）与其它集合的差集。需要注意的是并非获取全部集合的差集，二者存在部分差异。若集合不存在将被视为空集合。

```
SDIFF key [key ...]

```

官方文档中给出了下列示例：

```
key1 = {a,b,c,d}
key2 = {c}
key3 = {a,c,e}
SDIFF key1 key2 key3 = {b,d}

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SDIFF set1 set2
1) "1"
2) "2"

```

#### SDIFFSTORE

`SDIFFSTORE`命令与`SDIFF`命令相似，区别在于`SDIFFSTORE`命令不直接返回其差集，而是保存到`destination`参数指定的集合中，并返回结果的数量。若`destination`参数指定的集合已存在将会被覆盖。

```
SDIFFSTORE destination key [key ...]

```

###### 示例

```
redis> SADD set1 1 2 3
(integer) 3
redis> SADD set2 3 4 5
(integer) 3
redis> SDIFFSTORE set3 set1 set2
(integer) 2
redis> SMEMBERS set3
1) "1"
2) "2"
```
