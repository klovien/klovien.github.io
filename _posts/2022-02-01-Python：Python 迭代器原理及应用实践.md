---
layout:     post
title:      Python：Python 迭代器原理及应用实践
subtitle:   使用迭代器生成斐波那契数列
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---

# 1. 什么是迭代器

- 迭代是Python最强大的功能之一，是访问集合元素的一种方式；
- 迭代器是一个可以记住当前遍历的位置的对象；
- 迭代器获基于 **迭代协议** 取值，背后是 ```__iter()__``` 方法；

# 2. 迭代器如何工作

- 迭代器有两个基本的方法：```__iter()__``` 和 ```__next()__```；
  -- 注意：如果仅仅实现 ```__iter()__``` 方法，只能通过 ```for``` 循环来进行迭代，而如果想要通过 ```next``` 方法迭代的话则需要使用 ```__next()__```方法：
- 迭代器对象从集合的第一个元素开始访问，每访问一个元素就记录当前位置，然后当下一次访问元素的时候，就从当前位置继续访问下一个，直到所有的元素被访问完结束；
- 当所有元素都被迭代出来以后，迭代器中没有更多的元素，```__next()__``` 动作会抛出 ```StopIteration``` 的错误；
- 下面的例子中，我们引入 ```iter()``` 这个内置的迭代函数做一个演示，```iter()``` 是 Python 内置的可迭代函数，可以将 list、set、str 等类型对象转化成可迭代对象；
```
>>> list=[1,2,3,4]
>>> it = iter(list)    # 创建迭代器对象
>>> print (next(it))   # 输出迭代器的下一个元素
1
>>> print (next(it))
2
>>> print (next(it))
3
>>> print (next(it))
4
>>>
>>> print (next(it))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration                                      # 迭代器中没有更多的元素时，next() 动作会抛出 **StopIteration** 的错误
```

# 3. 迭代器与列表的区别

- 与 list 序列不同，迭代器不能按位置下标返回；
  -- list 下标获取值，背后的原理是 ```__getitem__```，而迭代器获取值用到的 **迭代协议**，背后是 ```__iter__``` 方法。
- 我们举一个例子：
>-- 有一只兔子，它在菜园里种了三十个胡萝卜，这只兔子每天都要吃一个胡萝卜，它如何为接下来的一个月准备口粮呢？分析一下我们发现，其实这只兔子有两种方法可以获取食物：
>
>  第一种方法，是直接准备一个月的食物，即把全部三十个胡萝卜拖进自己的洞里，然后每天吃一个，这种是常规方法，但是有个问题就是兔子洞不一定能放得下三十个胡萝卜；
>
>第二种方法，是每天从菜园里挖一个胡萝卜出来吃，每次吃完以后到记住胡萝卜坑的位置，第二天来的时候从这个萝卜坑的位置继续往下找，直到吃完三十个胡萝卜为止；
- 上面的例子中：
  -- 第一种方法其实对应的就是列表存储数据的方式，简单易懂但问题是三十根胡萝卜太占地方，也就是我们说的 **占用内存空间**；
  -- 第二种方法，其核心思想就是迭代器的设计思路，即迭代器中存储的不是元素数据，而是获取数据的方式（即数据存放的位置），每次获取一个数据然后记录位置，下一次获取数据的时候接着往下找；

# 4. 如何构造一个迭代器

- 如下所示，```IteraleSample``` 类中内置了 ```__iter__```、```__next__``` 两个方法，所以后面的判断 ```isinstance(sample, Iterator)``` 返回的结果是 ```True```；
```
from collections import Iterable, Iterator


class IteraleSample(object):
    '''
    创建一个类对象，并定义 __iter__、__next__ 两个方法
    '''
	def __init__(self):
		pass

	def __iter__(self):
		return self

	def __next__(self):
		pass

sample = IteraleSample()
print(isinstance(sample, Iterator))
```
- 打印结果如下：
```json
True
```
# 5.  __ iter __ 方法一定要 return 可迭代对象

- 需要指出的是，迭代器中 ```__iter__``` 方法一定要 ```return 一个可迭代对象```；
  -- 上面例子 ```__iter__``` 方法 ```return``` 的 ```self```，表示的是 ```IteraleSample``` 类自己，这个类是可迭代的；
- 如果 ```__iter__``` 方法没有 return 可迭代对象的话，会怎么样呢？
  -- 下面，我们构造了一个 ```WrongIteraleSample``` 类，该类的 ```__iter__``` 方法下只写一个 ```pass``` 占位符；
```json
# 错误的示例

from collections import Iterable, Iterator


class WrongIteraleSample(object):
	'''
	创建一个类对象，并定义 __iter__、__next__ 两个方法
	'''
	def __init__(self, max_num):
		self.current = 0
		self.max_num = max_num

	def __iter__(self):
		pass        # 【注意】__iter__ 方法没有 return 可迭代对象，这种写法是【错误的】！！！

	def __next__(self):
		# pass
		if self.current < self.max_num:
			self.current += 10
		return self.current

sample = IteraleSample(5)
print(isinstance(sample, Iterator))

for num in sample:
	print(num)
```
- 运行后发现，尽管 ```isinstance(sample, Iterator)``` 的返回值也是 ```True```，但在实例化该类后执行 ```for num in sample:
  print(num)``` 的时候，会报错 ```TypeError: iter() returned non-iterator of type 'NoneType'```；
```json
True
Traceback (most recent call last):
  File "C:\Users\Administrator\Desktop\iterator_test.py", line 52, in <module>
    for num in sample:
TypeError: iter() returned non-iterator of type 'NoneType'
```

# 6. 使用迭代器构造斐波那契数列
#### 6.1. 斐波那契数列简介
-- 斐波那契数列，是有一系列整数组成的数列；
-- 这一数列任意位置上的数值与后一位相加，等于第三位数值；
-- 示例：
```json
0 , 1 , 1 , 2 , 3 , 5 , 8 , 13 , 21 , 34 , ...
```
##6.2. 普通方法构造斐波那契数列
- 普通方法一：
  -- 如下面代码，使用递归构造斐波那契数列生成函数，并记录运行时间；
```
def feb_1(index):
    """
    斐波那契数列生成函数
    """
    if index <= 2:
        return 1
    elif index > 2 and index < 101:
        return feb_1(index-1) + feb_1(index-2)

def test_time(max_index):
    """
    测试上面 斐波那契数列函数的执行情况
    """
    starttime = datetime.datetime.now()  # 记录开始时间
    print("最大值为 ： {}\n结果值为：{}".format(max_index, feb_1(max_index)))
    endtime = datetime.datetime.now()  # 记录结束时间
    print("运行时间为：{}".format(endtime - starttime))  # 以秒为单位，计算运行时长
```
-- 方法缺陷：
（1）每次只能获取一个末位的数值，而不是整个数列；
（2）如果数值太大，运行时间会变得非常的长；
```
print(test_time(10))

print(test_time(20))

print(test_time(30))

print(test_time(40))

最大值为 ： 10
结果值为：55
运行时间为：0:00:00
None

最大值为 ： 20
结果值为：6765
运行时间为：0:00:00.003998
None

最大值为 ： 30
结果值为：832040
运行时间为：0:00:00.469920 
None

最大值为 ： 40
结果值为：102334155
运行时间为：0:00:57.805053       # 此处，max_index=40，运行时间就已经需要 57 秒，相比前面的时间花费增速是几何式的，
None

```
-- 如上面的例子，max_index = 30 的时候程序运行时常只需要 0:00:00.749871 秒，到了 max_index = 40 的时候就已经需要 0:00:57.805053 秒，这种耗时的增速在实际生产中是不能接受的；
- 普通方法二：
```
def feb_2(max_index):
    """
    斐波那契数列生成函数
    """
    result_list = []          # 此处使用 list 作为一个容器存储生成的数值，如果数值足够大这个 list 将会占用大量的的内存
    n, a, b = 0, 0, 1
    while n < max_index:
        result_list.append(b)
        a,b = b, a+b
        n += 1
    return "最大值为 ： {}\n结果值为：{}".format(max_index, result_list)

def test_time(max_index):
    """
    测试上面 斐波那契数列函数的执行情况
    """
    starttime = datetime.datetime.now()  # 记录开始时间
    print(feb_2(max_index))
    endtime = datetime.datetime.now()  # 记录结束时间
    print("运行时间为 ： {}".format(endtime - starttime))  # 以秒为单位，计算运行时长
```
-- 该方法的优点：相比与前面的方法，程序运行耗费的时间相对较快；
```
test_time(10)

test_time(20)

test_time(30)

test_time(40)

最大值为 ： 10
结果值为：[1, 1, 2, 3, 5, 8, 13, 21, 34, 55]
运行时间为 ： 0:00:00.001000

最大值为 ： 20
结果值为：[1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765]
运行时间为 ： 0:00:00

最大值为 ： 30
结果值为：[1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040]
运行时间为 ： 0:00:00

最大值为 ： 40
结果值为：[1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155]
运行时间为 ： 0:00:00
```

-- 该方法的缺陷：该方法使用 result_list = [] 作为一个容器存储生成的数值，如果数值足够大这个 list 将会占用大量内存空间，简言之：**如果数值过大，会占用大量内存**；
-- 有兴趣的小伙伴可以尝试 max_index=10000 时程序的执行情况，看一看程序内存占用情况；
-- 请注意：你的电脑可能会被卡死！！！
```
test_time(10000)    # 注意：你的电脑可能会被卡死
```
#### 6.3. 使用迭代器构造斐波那契数列
- 如下，我们使用迭代器实现一个斐波那契数列：
```
class Fibonacci(object):
	def __init__(self, max_num):
		self.max_num = max_num
		self.current = 0
		self.a = 0
		self.b = 1

	def __iter__(self):
        '''
        【注意】此处一定要 return self，否则会报错 TypeError: iter() returned non-iterator of type NoneType ！！！
        '''
		return self  

	def __next__(self):
		if self.current < self.max_num:
			self.a, self.b = self.b, self.a + self.b
			self.current += 1
			return self.a
		else:
			raise StopIteration


fib = Fibonacci(10)
for num in fib:
	print(num)
```
- 下面打印的结果正是我们想要的：
```json
1
1
2
3
5
8
13
21
34
55
```
- 我们来测试其执行效率
```
import datetime


class Fibonacci(object):
	def __init__(self, max_num):
		self.max_num = max_num
		self.current = 0
		self.a = 0
		self.b = 1

	def __iter__(self):
		return self

	def __next__(self):
		if self.current < self.max_num:
			self.a, self.b = self.b, self.a + self.b
			self.current += 1
			return self.a
		else:
			raise StopIteration


def test_time(max_num):
    """
    测试上面 斐波那契数列函数的执行情况
    """
    starttime = datetime.datetime.now() 
    for i in Fibonacci(max_num):                                   # 使用 for ... in ... 遍历打印所有的生成结果
        print(i)
    endtime = datetime.datetime.now() 
    print("运行时间为 ： {}".format(endtime - starttime))


test_time(10)        
test_time(100)
test_time(1000)
test_time(10000) 
```
- 运行耗时、内存占用都被大大缩减，下面的结果展示了运行时间结果，和前面的对比这种方法的效率是惊人的；
```
# 运行的时间如下所示：

运行时间为 ： 0:00:00
运行时间为 ： 0:00:00.004000
运行时间为 ： 0:00:00.574901
运行时间为 ： 0:00:04.990133
