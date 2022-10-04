---
layout:     post
title:      Python：*args 和 **kargs 的用法和区别
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. *args 和 **kargs 到底有什么用

>在 python 中，```*args``` 和 ```**kwargs``` 都用作向函数传递 **可变参数**。
所谓 **可变**，其实就是参数的 **数量**、**位置**、**形式** 是不固定的。
之所以使用可变参数，目的是为了在构造函数的时候，增加函数功能的灵活性，同时函数写法会更加优雅。

在编程工作中，经常需要传递很多参数到函数中，但这些参数 **有的时候需要用到、有时候则不需要用到**，甚至有时候参数根本不存在，最典型的例子就是 **构造装饰器**。

注意：如果看不懂可以跳过下面这一部分！

下面是一个 ```timer``` 装饰器，用来计算并打印函数执行花费的时间，在构造参数的时候用到了 ```*args``` 和 ```**kwargs``` 两个参数。
```
def timer(func):
...     def call_func(*args, **kwargs):
...         print(f"计时开始 [{time.strftime('%Y-%m-%d %X', time.localtime())}]")
...         start_time = time.time()
...         try:
...             func(*args, **kwargs)
...         except KeyboardInterrupt:
...             pass
...         finally:
...             end_time = time.time()
...             total_time = end_time - start_time
...             print("计时结束")
...             print(f"程序用时{int(total_time // 60)}分{total_time % 60:.2f}秒")
...     return call_func
... 
@timer
... def test(arg_1, arg_2, arg_3):
...     print("开始 执行 test 函数")
...     time.sleep(3)
...     print(f"打印参数 arg_1: {arg_1} arg_1: {arg_1} arg_1: {arg_1}")
...     print("执行 test 函数 结束")
...     
test(1, 2, 3)
计时开始 [2020-10-30 17:50:23]
开始 执行 test 函数
打印参数 arg_1: 1 arg_1: 1 arg_1: 1
执行 test 函数 结束
计时结束
程序用时0分3.01秒

``` 
上面的装饰器之所以传递 **可变参数**，是因为装饰器需要装饰的函数的数量是不固定的，如果传递固定的参数，那么被装饰的函数在传参的时候会报错。

# 2. *args 和 **kargs 用法有什么区别

##2.1. 可变位置参数 *args
- python 中规定参数前带 ```*``` 的称为可变位置参数，只是我们通常称这个可变位置参数为 ```*args```，我们也可以写成 ```*any``` 等形式。
- ```*args``` 是一个 **元组**，传入的参数会被放进元组里。

**场景一：**
- ```*args``` 将多个参数存储在 **元组** 类型的变量 ```args``` 当中，当我们在函数内部使用 ```args``` 获取参数的时候，会得到一个 **元组**。
>注意：如果在函数内部使用 ```*args``` 获取参数， ```*args``` 得到的结果 **并不是元组**，而是分开的多个参数值，这与 ```args``` 获取的结果是不一样的。
```
>>> def func_args(arg_1, arg_2, *args):
...     print(f"arg_1 : {arg_1}")
...     print(f"arg_2 : {arg_2}")
...     print(f"args : {args}")
...     print(f"*args : {*args}")    # 注意这里获取的是 *args
...
>>> func_args('a', 'b', 'c', 'd', 'e')
arg_1 : a
arg_2 : b
args : ('c', 'd', 'e')
*args : 'c', 'd', 'e'         # 注意这里打印的 并不是元组
>>>
```
- 上面代码中，```arg_1```、```arg_2``` 是固定参数，传参的时候函数会将第一、二个参数按位置赋给 ```arg_1```、```arg_2```，就是代码中的 ```a```、```b```。
- 而后面 ```c```、```d```、```e```，则一并存储为 **元祖** 传给 ```args```，所以才会在打印的结果中看到打印的返回值：```args : ('c', 'd', 'e')```。

**场景二：**
- 用于容器传参，即函数被调用时传入了一个容器（如 list），容器中包含多个参数，而这些参数分别对应函数的参数。
```
def func(arg1, arg2, arg3):
...     print(arg1, arg2, arg3)
... 
... arg_list=["second", 3]
... func(1, *arg_list)       # 此处用 *arg_list 这个 list 传入 "second" 和 3 两个参数
1 second 3
```
- 在上面的例子中，```func(1, *arg_list) ``` 其实就等效于 ```func(1, "second", 3) ```，这种写法其实并没有特别的作用，只是让代码看起来更优雅。
>**注意**：用的时候注意，调用时 **传入的参数总量**（即容器外参数和容器内元素个数的总和），必须和 **构造函数所需的参数数量** 保持一致。以上面的例子为例，构造函数时 ```arg1, arg2, arg3``` 共计 **三个** 参数，调用函数时传入 ```1, "second", 3``` 加起来也是 **三个**。 如果数量不一样，传入的太多或者太少都会报错。

下面是一个例子：
```

... arg_list=["second", 3, 4, 5]		# 传递的参数 太多
... func(1, *arg_list)
Traceback (most recent call last):
  File "<input>", line 5, in <module>
TypeError: func() takes 3 positional arguments but 5 were given  
... 
... arg_list=["second",]				# 传递的参数 太少
... func(1, *arg_list)
Traceback (most recent call last):
  File "<input>", line 6, in <module>
TypeError: func() missing 1 required positional argument: 'arg3'
```

##2.2. 可变关键字参数 **kargs
- python 中规定参数前 带 ```**``` 的，称为可变关键字参数，通常用 ```**kwargs``` 表示。
- ** kwargs：是一个字典，传入的参数以 **键值对** 的形式存放到字典里，相比较而言，*args 是一个 **元组**，到这里读者应该明白二者的区别了吧。

**场景一：**
- **kargs 实质就是将函数的参数和值，存储在 **字典类型** 的 kargs 变量中。
```
def func_kwargs(arg_1, **kwargs):
...     print(f"第一个参数 arg_1 : {arg_1}")
...     print(f"后面的参数 kwargs ： {kwargs}")
...     
func_kwargs('a', arg_2='b', arg_3='c', arg_4='d')    # 注意参数格式一定写成 key=value 形式
第一个参数 arg_1 : a
后面的参数 kwargs ： {'arg_2': 'b', 'arg_3': 'c', 'arg_4': 'd'}
```
在上面的例子中，``` arg_2='b', arg_3='c', arg_4='d'```的含义是：```arg_2、arg_3、arg_4``` 三个参数的值分别为 ```'b'、'c'、'd'```，在参数传入函数后，他们一起被构造成一个字典，即打印出的 ```kwargs ： {'arg_2': 'b', 'arg_3': 'c', 'arg_4': 'd'}```。

>**注意**：传入的可变参数一定要写成 **```key=value```** 的形式，否则会报 **赋参太多** 错误。

详见下面的赋参太多错误的示例：
```
def func_kwargs(arg_1, **kwargs):
...     print(f"第一个参数 arg_1 : {arg_1}")
...     print(f"后面的参数 kwargs ： {kwargs}")
... 
func_kwargs('a', 'b', 'c', 'd')          # 直接传递参数
Traceback (most recent call last):
  File "<input>", line 1, in <module>
TypeError: func_kwargs() takes 1 positional argument but 4 were given
```
**场景二：**
- **kargs的第二种使用场景，与 *args 第二种使用场景类似，也是用于传递参数到函数中，只不过区别是  *args 可以传入的是列表、元组等，而 **kargs 传入的是字典。

```
def func(arg_1, arg_2=None, arg_3=None):
...     print(arg_1, arg_2, arg_3)
... 
arg_dic={"arg_2": "第二个参数的值", "arg_3": "第三个参数的值"}    # 需要传入的字典
func("第一个参数", **arg_dic)
第一个参数 第二个参数的值 第三个参数的值
```
在上面的例子中，```arg_dic={"arg_2": "第二个参数的值", "arg_3": "第三个参数的值"}``` 以字典形式传入函数 ```func```，在传入后自动匹配赋值： ```arg_2="第二个参数的值", arg_3="第三个参数的值"```。
>**注意**：在构造函数的时候，参数 **不一定** 要写成 **```key=value```** 的形式，按照普通函数的形式来写，也能正常接收传入的字典形式的参数。

下面是普通的函数构造写法的例子：
```
def func_1(arg_1, arg_2, arg_3):    # 普通的函数参数写法
...     print(arg_1, arg_2, arg_3)
...     
arg_dic={"arg_2": "第二个参数的值", "arg_3": "第三个参数的值"}
func_1("第一个参数", **arg_dic)
第一个参数 第二个参数的值 第三个参数的值
```
- 至于参数的数量，情况和 ```*args``` 一样，传入字典的元素数量和函数所需的参数数量需要保持一致，否则会报错。
  下面是一个例子：
```
def func(arg_1, arg_2=None, arg_3=None):
...     print(arg_1, arg_2, arg_3)
...     
arg_dic={"arg_2": "第二个参数的值", "arg_3": "第三个参数的值", "arg_4": "第四个参数的值"}
func("第一个参数", **arg_dic)
Traceback (most recent call last):
  File "<input>", line 1, in <module>
TypeError: func() got an unexpected keyword argument 'arg_4'
```

# 3. *args 和 **kargs 一起使用

- 这里参考网上的帖子，给出了一个比较经典的例子。
- 有兴趣的朋友可以好好看一下其中的细节，尤其是 **仔细看一下注释** 内容。
```
def func(arg_1, arg_2="第二个参数的值", *args, arg_3="第三个参数的值", arg_4, **kwargs):
...     print(arg_1)     # [0, 1, 2]
...     print(arg_2)     # 3          # 注意：这里的 3，对应构造函数的 arg_2，它是参数 *[3, 4, 5] 传进来的！！！
...     print(args)      # (4, 5)
...     print(*args)     # 4 5        # 注意：这里返回的并不是元组
...     print(arg_3)     # 6
...     print(arg_4)     # 7
...     print(kwargs)    # {'key_1': '第一个字典参数', 'key_2': '第二个字典参数'}
...     # print(**kargs)
...     print(kwargs["key_1"])    # 第一个字典参数
...     print(kwargs["key_2"])    # 第二个字典参数

func([0, 1, 2], *[3, 4, 5], arg_3=6, arg_4=7, key_1="第一个字典参数", key_2="第二个字典参数")

[0, 1, 2]
3             
(4, 5)
4 5
6
7
{'key_1': '第一个字典参数', 'key_2': '第二个字典参数'}
第一个字典参数
第二个字典参数
```