---
layout:     post
title:      Python：Python 魔法函数
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. 简介
#### 1.1. 什么是魔法方法
- Python是一门追求简单、高效的语言，它并不像某些语言如Java、C++等，需要让类实现接口并逐一实现接口中的方法。Python采用了一种约定的机制，在基类中以特殊名称的方法、属性（类似 __ init __ 这样的属性和方法）来提供指定的功能；
- 在Python中有些方法名、属性名前后加上了双下划线，这种方法、属性通常属于Python类的特殊方法和属性，有的书中成为“魔法方法”；
#### 1.2. 调用方式
- 隐式调用
  - 魔法方法是 **隐式调用** 的，在使用的时候不需要调用方法
- 显示调用
  --
# 2. 简单实例
- __ init __(self, [...])
  - 类的初始化方法。它获取任何传给构造器的参数（比如我们调用 x = SomeClass(10, ‘foo’) ， __init__ 就会接到参数 10 和 ‘foo’ ，__init__ 在Python的类定义中用的最多；
- __ getitem __(self, key)
  - 定义对容器中某一项使用 self[key] 的方式进行读取操作时的行为。这也是可变和不可变容器类型都需要实现的一个方法；
- 示例：
```
class Compalny(object):
    # 类的初始化方法，获取参数 employee_list 这个列表
    def __init__(self, employee_list):
        self.employee = employee_list

    # 对 self.employee 这个 list 使用 self[key] 的方式进行读取操作
    def __getitem__(self, item):
        return self.employee[item]

company = Compalny(["tom", "bob", "jane"])

# 遍历 company 中所有员工
for em in company:
    print(em)

print('=' * 50)

# 获取 company 中的第三个元素
print(company[2])
```
# 3. 常用魔法方法
- 常用魔法方法
  - __ init __：类实例化会触发；
- __ repr __
  - 和 __ init __(self) 的性质一样，Python 中每个类都包含 __ repr __() 方法，因为 object 类包含 __ reper __() 方法，而 Python 中所有的类都直接或间接继承自 object 类；
  - 示例：
```
>>> class User():
...     def __init__(self, name):
...         self.name = name
...     def __repr__(self):
...         return "user with name : "+ self.name
...
>>> user = User(name="zhangsan")
>>> print(user)
user with name : zhangsan      # 自动打印出 __repr__ 方法 return 的文字 
```

- __ str __：打印对象会触发；
- __ call __：对象()触发，类也是对象  类(),类的实例化过程调用元类的__call__；
- __ new __：在类实例化会触发，它比__init__早（造出裸体的人，__init__穿衣服）；
- __ del __：del 对象，对象回收的时候触发；
- __ setattr __ ， __ getattr __：(.拦截方法)，当对象.属性--》赋值会调用setattr，如果是取值会调用getattr；
- __ getitem __ ， __ setitem __：([]拦截)；
- __ enter __ 和  __ exit __ 上下文管理器；