---
layout:     post
title:      Python：Python生成器原理及应用实践
subtitle:   用生成器构建斐波那契数列和杨辉三角
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---

## 生成器函数

#### 什么是生成器

- 在 Python 中，一边循环一边计算的机制，称为生成器（Generator）；
- 生成器是一个返回迭代器的函数，只能用于迭代操作；
- 生成器可以通过生成器表达式和生成器函数获取到；
- 生成器是 Python 中的一个对象，对这个对象进行操作，可以依次生产出按生成器内部运算规则产生的数据；
- **生成器，最大的特点，就是可以 暂停 \ 恢复**；

#### 迭代器与生成器的关系

> 【说明】：很多文章在介绍生成器的时候，上来就引入 ```yield```，说使用 ```yiled``` 关键字的函数就是生成器， 但很多人并不能理解 ```yield``` 的含义，这种用新概念解释新概念的方式，对于新人来说很不友好，这里我先从最基本的迭代器开始，介绍如何用迭代器构造一个生成器，进而引入 ```yiled``` 概念。

- ```生成器，本质上仍然是一个迭代器```，它是对迭代器的调用，同时自身也是一个迭代器，符合迭代器的构造规范；
  - 不了解迭代器的朋友，可以参考我的另一篇文章《[Python：理解迭代器，并用迭代器生成斐波那契数列](https://www.jianshu.com/p/5dc09fcee57a)》；

- 生成器是一种特殊的迭代器，与迭代器不同的是，```生成器是被动生成```，每被调用一次则生成一次；

- 下面，我们用迭代器实现了一个生成器函数，：
  - 其中 ```IteraleSample``` 是一个可迭代类，```Generator``` 是一个 生成器类；
  - ```Generator``` 通过 ```iterator_sample``` 属性调用 ```IteraleSample``` 类；
  - 同时 ```Generator``` 将自身的 ```max_generate_num``` 属性值赋值给 ```IteraleSample``` 的 ```max_iter_num```：

    ```
    class IteraleSample(object):
        '''
        创建一个类对象，并定义 __iter__、__next__ 两个方法
        '''
        def __init__(self, max_iter_num):
            '''
            max_iter_num: 迭代器最大迭代次数
            '''
            self.current = 0
            self.max_iter_num = max_iter_num
    
        def __iter__(self):
            return self
    
        def __next__(self):
            # pass
            if self.current < self.max_iter_num:
                self.current += 1
            else:
                raise StopIteration
            return self.current
    
    
    class Generator(object):
        def __init__(self, max_generate_num):
            '''
            max_generate_num: 生成器最大生成次数
            '''
            self.max_generate_num = max_generate_num
            self.iterator_sample = IteraleSample(self.max_generate_num)
    
        def __iter__(self):
            return self.iterator_sample
    
        def __next__(self):
            return self.iterator_sample.__next__() * 10
    
    
    gen = Generator(10)    # 实例化生成器，并赋值最大迭代次数为 10
    
    print(next(gen))       # 使用 next 函数迭代元素
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))
    print(next(gen))        # 此处，共计 next 迭代 11 次，超出了生成器的最大迭代次 10
    ```
  - 上面代码的执行结果如下，分析发现每次 ```next(gen)``` 都会迭代出一个结果值，这个结果值就是 ```IteraleSample``` 迭代出的元素（从1到10）乘以 10 的结果，而第十一次迭代时则报 ```StopIteration``` 异常；
    ```
    10
    20
    30
    40
    50
    60
    70
    80
    90
    100
    Traceback (most recent call last):
      File "C:\Users\Administrator\Desktop\iterator_test.py", line 94, in <module>
        print(next(gen))
      File "C:\Users\Administrator\Desktop\iterator_test.py", line 76, in __next__
        return self.iterator_sample.__next__() * 10
      File "C:\Users\Administrator\Desktop\iterator_test.py", line 63, in __next__
        raise StopIteration
    StopIteration
    ```
#### 如何构造生成器
  
- 前面如果能看懂，其实生成器就已经理解的产不多了，```yield``` 关键字构造生成器函数只是一种比较简洁的写法罢了；
- 如果前面看不懂也没关系，只要记住一点就好了：**只要函数里面有 ```yield``` 关键字，就是生成器函数**；
  -- 很多人看到这里时有点晕，因为人不懂什么是 ```yield```，我们
- 生成器函数保存的是函数内部的算法，每次调用就通过算法计算出下一个元素的值，直到计算到最后一个元素；
- 生成器函数使用 **next()** 调用生成器函数的赋值对象（不是函数本身）返回 yiled 值；
    
    ```
    >>> def fun_gen():
    ...     print("第一次 yield")
    ...     yield 1
    ...     print("第二次 yield")
    ...     yield 2
    ...     print("第三次 yield")
    ...     yield 3
    ...
    >>> result = fun_gen()                # 注意：必须将生成器函数赋值于某个变量！！！
    >>> next(result)                      # next 调用的是被赋值后的变量，而不是函数本身；
    第一次 next
    1
    >>> next(result)                      # next 调用的是被赋值后的变量，而不是函数本身；
    第二次 next
    2
    >>> next(result)                      # next 调用的是被赋值后的变量，而不是函数本身；
    第三次 next
    3
    ```
  >注意：生成器函在被 next 调用之前，必须将生成器函数赋值于某个变量，next 调用的是被赋值后的变量，而不是函数本身，如：下面代码中的 result = fun_gen()，这一步一定不能省，next 调用的是 result，不是 fun_gen()；

- 与迭代器一样，生成器中没有更多的元素时，next() 动作会抛出 **StopIteration** 的错误；

    ```
    >>> def fun_gen():
    ...     print("第一次 yield")
    ...     yield 1
    ...     print("第二次 yield")
    ...     yield 2
    ...     print("第三次 yield")
    ...     yield 3
    ...
    >>>
    >>> result = fun_gen()
    >>> next(result)
    第一次 yield
    1
    >>> next(result)
    第二次 yield
    2
    >>> next(result)
    第三次 yield
    3
    >>> next(result)
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    StopIteration                          # 没有更多的元素，next() 动作抛出 StopIteration 的错误
    ```

- 实际使用生成器函数的时候，不必依次进行 next 调用，一般使用 for...in...，这样会更简洁、合理；
    ```
    >>> def fun_gen():
    ...     print("第一次 yield")
    ...     yield 1
    ...     print("第二次 yield")
    ...     yield 2
    ...     print("第三次 yield")
    ...     yield 3
    ...
    >>>
    >>> for i in fun_gen():
    ...     print(i)
    ...
    第一次 yield
    1
    第二次 yield
    2
    第三次 yield
    3
    ```

#### 生成器函数 与 普通函数 的区别

- 生成器函数可以多次 yield 结果值，普通函数只能返回一次结果值；
- 生成器函数需要结果值的时候就直接生成一个，不需要直接占用很大的内存空间，而普通函数执行时需要足够的内存空间，尤其是需要生成元素很多的列表、巨大的数值的时候，需要占用巨大的内存空间；
- 生成器函数仅仅是需要的时候才使用，有点像数据库操作单条记录使用的游标；
- 如果要读取并使用的内容远远超过内存，但是需要对所有的流中的内容进行处理，那么生成器是一个很好的选择，可以让生成器返回当前的处理状态，由于它可以保存状态，那么下一次直接处理即可；
- 相比于普通函数，生成器函数最大的优点在于： ```生成器函数处理集合的效率远远优于普通函数，内存占用低、耗时短```；

## 用生成器构造斐波那契数列

#### 斐波那契数列简介
- 斐波那契数列，是有一系列整数组成的数列；
- 这一数列任意位置上的数值与后一位相加，等于第三位数值；
- 示例：
    ```
    0 , 1 , 1 , 2 , 3 , 5 , 8 , 13 , 21 , 34 , ...
    ```
#### 普通方法构造斐波那契数列
- 普通方法一：
  - 如下面代码，使用递归构造斐波那契数列生成函数，并记录运行时间；
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
    
  - 方法缺陷： 
    - 每次只能获取一个末位的数值，而不是整个数列； 
  - 如果数值太大，运行时间会变得非常的长；
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
    
  - 分析上面的例子：
    - max_index = 30 的时候程序运行时常只需要 0:00:00.749871 秒；
    - 到了 max_index = 40 的时候就已经需要 0:00:57.805053 秒，这种耗时的增速在实际生产中是不能接受的；

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
  - 该方法的优点：相比与前面的方法，程序运行耗费的时间相对较快；
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

  - 该方法的缺陷：该方法使用 result_list = [] 作为一个容器存储生成的数值，如果数值足够大这个 list 将会占用大量内存空间，简言之：**如果数值过大，会占用大量内存**；
  - 有兴趣的小伙伴可以尝试 max_index=10000 时程序的执行情况，看一看程序内存占用情况；
  - 请注意：你的电脑可能会被卡死！！！
    ```
    test_time(10000)    # 注意：你的电脑可能会被卡死
    ```

#### 生成器构造斐波那契数列

- 如下面的代码：
  - fib_3(max_index) 是用生成器函数构成的斐波那契数列生成函数；
  - 在 test_time(max_index) 测试函数中使用 for i in fib_3(max_index) print(i) 打印所有的生成结果；

    ```
    import datetime
    
    
    def fib_3(max_index):
        """
        使用 yiled 生成器构造的斐波那契数列生成函数
        """
        n, a, b = 0, 0, 1
        while n < max_index:
            yield b                                                 # 此处使用 yiedl b 
            a,b = b, a+b
            n += 1
    
    
    def test_time(max_index):
        """
        测试上面 斐波那契数列函数的执行情况
        """
        starttime = datetime.datetime.now() 
        for i in fib_3(max_index):                                   # 使用 for ... in ... 遍历打印所有的生成结果
            print(i)
        endtime = datetime.datetime.now() 
        print("运行时间为 ： {}".format(endtime - starttime))
    
    
    test_time(10)        
    test_time(100)
    test_time(1000)
    test_time(10000) 
    ```

- 下面的结果展示了运行时间结果，；
```
# 运行的时间如下所示：

运行时间为 ： 0:00:00
运行时间为 ： 0:00:00
运行时间为 ： 0:00:00.001000
运行时间为 ： 0:00:00.004998

```

#### 生成器函数构建杨辉三角

- 杨辉三角简介：
  - 杨辉三角，是二项式系数在三角形中的一种几何排列，中国南宋数学家杨辉1261年所著的《详解九章算法》一书中出现；
  - 在欧洲，帕斯卡在1654年发现这一规律，所以这个表又叫做[帕斯卡三角形；
  - 杨辉三角示例如下图：

![20190307104242486.png](https://upload-images.jianshu.io/upload_images/14502986-8f1d1a6652aa0be0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 杨辉三角特性：
  - 每个数等于它上方两数之和；
  - 每行数字左右对称，由1开始逐渐变大；
- 代码实现：
    ```
    def triangles(max_index):        # max_index 表示最大行数
        L = [1]
        i = 1
        while i <= max_index:        # 进入持续循环
            yield L[:len(L)]         # 输出数组
            L.append(0)              # 为数组添加最后一位
            L = [L[j - 1] + L[j] for j in range(len(L))]
            i += 1
    
    for i in triangles(10):          # 使用 for 循环打印结果值
        print(i)

    ```
    - 运行结果
    ```
    [1]
    [1, 1]
    [1, 2, 1]
    [1, 3, 3, 1]
    [1, 4, 6, 4, 1]
    [1, 5, 10, 10, 5, 1]
    [1, 6, 15, 20, 15, 6, 1]
    [1, 7, 21, 35, 35, 21, 7, 1]
    [1, 8, 28, 56, 70, 56, 28, 8, 1]
    [1, 9, 36, 84, 126, 126, 84, 36, 9, 1]
    ```