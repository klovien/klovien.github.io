---
layout:     post
title:      Python：Python 语言中的序列
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. Python 序列分类

#### 1.1. 按存储数据类型分类

- 容器序列，
  -- 定义：容器序列可以存放不同类型的数据，即可以存放任意类型对象的引用；
  -- 包括：list，tuple，deque；
- 扁平序列
  -- 定义：扁平序列只能容纳一种类型，也就是说其存放的是值而不是引用，扁平序列其实是一段连续的内存空间，由此可见扁平序列其实更加紧凑。但是它里面只能存放诸如字符、字节和数值这种基础类型。
  -- 包括：str，bytes，bytearray，array.array；

####  1.2. 按是否可变分类

- 可变序列
  -- list，deque，bytearray，array；
- 不可变序列
  -- str，tuple，bytes；

# 2. append & extend

- append
  -- append 对不同类型的序列进行新增操作，是把新增序列作为一个元素添入；
```
>>> a = [1, 2, 3]
>>> b = (4, 5)
>>>
>>> a.append(b)
>>> print(a)
[1, 2, 3, (4, 5)]      # 注意：(4, 5) 是一个元组
```
- extend
  -- extend 对不同类型的序列进行新增操作，会先 for 循环遍历新增序列中的元素，然后把这些元素逐个添入；
```
>>> a = [1, 2, 3]
>>> b = (4, 5)
>>>
>>> a.extend(b)
>>> print(a)
[1, 2, 3, 4, 5]        # 注意：4, 5 作为单独的元素出现在新列表
```

# 3. 序列切片

- 列表全部元素组成新列表
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[::])
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
- 列表全部元素组成 **倒序** 新列表
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[::-1])
[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
```
- **奇数位** 元素组成新列表
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[::2])    # 2 为步长，表示每隔一位取一个元素
[1, 3, 5, 7, 9]
```
- **偶数位** 元素组成新列表
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[1::2])
[2, 4, 6, 8, 10]
```
- **指定起始结束位置** 截取列表
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[3:6])
[4, 5, 6]
```
- 切片 **结束位大于列表长度**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[0:100])
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
- 切片 **初始位大于列表长度**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>>
>>> print(sample_list[100:])
[]
```
- **尾部增加** 新元素
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[len(sample_list):] = ["new"]
>>> print(sample_list)
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 'new']
```
- **头部增加** 新元素
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[:0] = ["new_1", "new_2"]
>>> print(sample_list)
['new_1', 'new_2', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```
- **指定位置** 增加元素
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[3:3] = ["new_1"]
>>> print(sample_list)
[1, 2, 3, 'new_1', 4, 5, 6, 7, 8, 9, 10]
```
- 替换 **指定起始位置** 多位元素，= 号两边元素 **数量相同**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[:3] = ["new_1", "new_2", "new_3"]
>>> print(sample_list)
['new_1', 'new_2', 'new_3', 4, 5, 6, 7, 8, 9, 10]
```
- 替换 **指定起始位置** 多位元素，= 号两边元素 **数量不同**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[:3] = ["new_1", "new_2"]        # =号左侧 3 个元素，=号右侧 2 个元素
>>> print(sample_list)
['new_1', 'new_2', 4, 5, 6, 7, 8, 9, 10]        # 注意：3 没有了，但并没有被替换成新元素
```
- **奇数位** 替换 **同一元素**，= 号两边元素 **数量相同**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[::2] = [0] * 5        
>>> print(sample_list)
[0, 2, 0, 4, 0, 6, 0, 8, 0, 10]
```
- **奇数位** 替换 **同一元素**，= 号两边元素 **数量不同**，会报错！
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[::2] = [0] * 3                  # = 号左侧 5 个元素，=号右侧 3 个元素
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: attempt to assign sequence of size 3 to extended slice of size 5
```
- **奇数位** 替换 **不同元素**，= 号两边元素 **数量相同**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[::2] = ["a", "b", "c", "d", "e"]
>>> print(sample_list)
['a', 2, 'b', 4, 'c', 6, 'd', 8, 'e', 10]
```
- **奇数位** 替换 **不同元素**，= 号两边元素 **数量不同**
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[::2] = ["a", "b", "c"]        # = 号左侧 5 个元素，=号右侧 3 个元素
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: attempt to assign sequence of size 3 to extended slice of size 5
```
- **删除指定起始位置** 元素
```
>>> sample_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
>>> sample_list[:3] = []                      # 删除前三位
>>> print(sample_list)
[4, 5, 6, 7, 8, 9, 10]
```

# 4. array 数组

- 与 list 不同，array 只能存放指定的数据类型；
- array 的性能要比 list 高很多；
