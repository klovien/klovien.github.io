---
layout:     post
title:      Python：动态属性 property & setter 以及 __getattr__ 属性
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. property
- 引言：
  - 假设有这样一个需求，我们需要创建一个 User 类，并初始化 birthday 参数，之后根据 birthday 计算得到年龄；
  - 我们设计下面的代码实现该需求：
```
>>> from datetime import date, datetime
>>>
>>>
>>> class User():
...     def __init__(self, nbirthday):
...         self.birthday = birthday
...     def get_age(self):
...         return datetime.now().year - self.birthday.year
...
>>>
>>> user = User(date(year=1990, month=3, day=5))
>>> age = user.get_age()
>>> print(age)
30
```
- 上面的代码中使用 user 实例调用 get_age() 方法获取 age，但是这种方法并不是常规方法，我们习惯用类似于 user.age 这样的方式获取 age；
- property

- 于是我们引入 property 属性，将 age() 方法变为实例的一个 age 属性，这样就可以通过 user.age 直接调用，代码如下：
```
>>> from datetime import date, datetime
>>>
>>>
>>> class User():
...     def __init__(self, birthday):
...         self.birthday = birthday
...     @property
...     def age(self):
...         return datetime.now().year - self.birthday.year
...
>>>
>>> user = User(date(year=1990, month=3, day=5))
>>> print(user.age)
30
```
# 2. setter
- 引言
  - 假设有这样一个需求，我们需要创建一个 User 类，初始化属性 age 默认为 0，之后改变 age 的值，并获取该值；
  - 我们设计下面的代码实现该需求：
```
>>> class User():
...     def __init__(self):
...         self.age = 0
...     def set_age(self, age):
...         self.age = age
...
>>>
>>> user = User("zhang")
>>> user.set_age(30)
>>> print(user.age)
30
```
- 上面的代码中使用 user 实例调用 set_age() 方法赋值于 age，但是这种方法并不是常规方法，我们习惯用类似于 user.age = xxx 这样的方式赋值 age；
- setter

- 于是我们引入 setter 属性，这样就可以通过 user.age 直接赋值，代码如下：
```
>>> class User():
...     def __init__(self,):
...         self._age = 0
...     @ property
...     def age(self):
...         return self._age
...     @age.setter
...     def age(self, age_value):
...         self._age = age_value
...
>>>
>>> user = User()
>>> user.age = 30
>>> print(user.age)
30
```

# 3. __ getattr __
- 引言
  - 在开发过程中，我们经常需要设计并获取类的实例对象的属性，但有时会因为疏忽遗漏没有为类设计某个属性，这就导致获取属性值的实收出现报错，故而造成程序异常甚至崩溃；
  - 如下面的代码：
```
>>> class User():
...     def __init__(self, name):
...         self.name = name
...
>>>
>>> user = User(name="zhangsan")
>>> print(user.age)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'User' object has no attribute 'age'
```
- __ getattr __ **处理 object has no attribute 报错**
  - 如上面的代码，User 类并没有 age 属性，所以在实例 user 获取 age 属性时遇到报错，这个时候需要用到 __ getattr __ 解决此问题；

```
>>> class User():
...     def __init__(self, name):
...         self.name = name
...     def __getattr__(self, item):      # 注意：此处 item 不能少
...         pass
...
>>>
>>> user = User(name="zhangsan")
>>> print(user.age)
None
```
- 在上面的代码中，User 类并没有 age 属性，所以在实例 user 获取 age 属性时并未报错，而只是反悔了一个 None，这就有效避免了报错；
- __ getattr __ **获取 dict 类型属性的 value 值**
  - 除了上面的用法，__ getattr __ 还可以用来更好的获取 dict 类型属性的 value 值；
  - 比如我们初始化类时给与其一个 dict 字典属性参数，我们希望可以通过 实例. 属性（比如 user.age）的方式直接获取  dict 字典中 key 为 age 的 value 值；
  - 以下面的代码为例：
```
>>> class User():
...     def __init__(self, info={}):
...         self.info = info
...     def __getattr__(self, item):
...         if not item in self.info.keys():
...             return None
...         return self.info[item]
...
>>>
>>> user = User({"name":"zhangsan", "age":23})
>>> print(user.age)
23
>>> print(user.gender)
None
```
- 代码中 user.age 的方式直接获取初始化参数 dict 字典中 key 为 age 的 value 值 23，同时 dict 中并未有 key 为 gender 的键值对，user.gender 直接返回 None 而并未出现报错； 