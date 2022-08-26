---
layout:     post
title:      Python：yield 高级用法 send、yiled from
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. yield 中 send 用法
- send 作用
  -- 生成器函数返回值给调用方；
  -- 调用方通过  send 传入值给生成器函数；
- 示例：

```
>>> def name_func():
...     yield_result = 0
...     while 1:
...         yield_result += 1
...         send_value = yield yield_result                      # 这一句是关键！！！
...         print(f"func has been sent value : {send_value}")
...
>>>
>>> gen = name_func()
>>> # print(next(gen))
... print("yield_result : ", gen.send(None))                     # 第一次调用, send 传入 None
yield_result :  1
>>> print("yield_result : ", gen.send("lavra"))
func has been sent value : lavra
yield_result :  2
>>> print("yield_result : ", gen.send("hiton"))
func has been sent value : hiton
yield_result :  3
```
> 上面的代码示例中，最关键的一句是：send_value = yield yield_result，其中：
-- send_value 是被 send 传入函数的值；
-- yield_result 是返回给调用方 gen 的值；
- next 和 send 的区别:
  -- next 只能取值；
  -- send 能取本次yield的返回值，也能给上次yield传入值；

- send 注意点：
  -- 第一个 send 不能给 yield 传值，必须要 send None，这一步不能省！

- 应用示例
```
>>> def url_repository():
...     url_list = []
...     while True:
...         # _url 是 send 发送给 yield 的参数值，在接下来的语句中使用，
...         _url = yield url_list                   # url_list是 yield 的返回值
...         url_list.append(_url)
...         print(f"ADD URL : {_url}")
...
>>>
>>> g = url_repository()                            # 实例化生成器函数
>>> g.send(None)                                    # 第一次调用, send 传入 None
[]
>>> print("URL REPO : ", g.send('url_1'))           # g.send() 返回 url_list
ADD URL : url_1
URL REPO :  ['url_1']                               # 打印 yield 返回的 url_list
>>> print("URL REPO : ", g.send('url_2'))
ADD URL : url_2
URL REPO :  ['url_1', 'url_2']
>>> print("URL REPO : ", g.send('url_3'))
ADD URL : url_3
URL REPO :  ['url_1', 'url_2', 'url_3']
```
# 2. yiled from 用法
- 下面是一个 yiled 生成器：
```
>>> def func():
...     for num in range(0, 5):
...         yield num
...
>>>
>>> g = func()
>>> print(next(g))
0
>>> print(next(g))
1
>>> print(next(g))
2
>>> print(next(g))
3
>>> print(next(g))
4
>>> print(next(g))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```

- 我们可以将其改成如下代码：
```
>>> def func():
...     yield from range(5)
...
>>>
>>> g = func()
>>> print(next(g))
0
>>> print(next(g))
1
>>> print(next(g))
2
>>> print(next(g))
3
>>> print(next(g))
4
>>> print(next(g))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```
