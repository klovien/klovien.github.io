---
layout:     post
title:      Scrapy：Scrapy 中 yield 和 return 的区别
subtitle:   
date:       2022-10-01
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: false
tags:
    - Scrapy
---


# 1.yiled 应用场景：

在 scrapy 中 yield 经常被使用，典型的应用场景包括以下两种：

- 场景一：

    ```
    yield scrapy.Reuqest(url=xxx, callback=self.xxx, meta={"xxx": xxx})
    ```

- 场景二：

    ```
    item= XXX()
    item["xxx"] = response.xpath("xxx").extract_first()
    yield item
    ```

# 2. yield 概念解析

- yiled 是一个迭代器，可被迭代调用；
- 要理解迭代器，首先需要注意一点，**迭代器应用在循环中**；
- 例如：

    ```
    def func_1():
        for i in range(5):
            yield f"result : {i}"
    ```

- 上面的func() 这个函数就是一个迭代器，在 ***for i in range(5)*** 这个循环中，迭代调用 ***result : {i}*** 这个表达式；
- 这样一个迭代器如何使用呢？我们需要 **将 func() 函数实例化**，并需要用到一个工具 ***next()***，如下：

    ```
    result = func_1()    # 此处将 func() 函数实例化
    
    next(result)     # 使用 next 调用实例化后的生成器
    'result : 0'     # 得到【第一次】生成的结果 0
    next(result) 
    'result : 1'     # 得到【第二次】生成的结果 1
    next(result) 
    'result : 2'     # 得到【第三次】生成的结果 2
    next(result)   
    'result : 3'     # 得到【第四次】生成的结果 3
    next(result) 
    'result : 4'     # 得到【第五次】生成的结果 4
    ```

- 到这里你就应该理解 yield 到底是个什么东西了，但你可能还会有问题，这个东西到底有什么用？它存在的意义是什么？这就需要和另一个 同类工具 return 作对比了。

# 3. yiled 与 return 的区别

- yield 和 return 类似，都是返回一个对象；
- yiled 和 return 的【区别】是：
  -- yield 返回的是生成器，返回一个对象以后，循环不会退出，还会继续执行下一个调用；
  -- return 则直接返回结果以后，就退出循环；
- 例如：

    ```
    def func_2():
        for i in range(5):
            return f"result : {i}"
    ```

- 对于 func_2() 函数，***for i in range(5)*** 这个循环，事实上只会循环一次，就直接 return 结果了；
- func_2() 不需要 next 调用，因为不是生成器，直接调用就好，而且当它被调用的时候，它 **只会执行一次**！！！

    ```
    func_2()
    'result : 0'    # 得到【第一次】生成的结果 0
    func_2()
    'result : 0'    # 得到【第二次】生成的结果 0
    func_2()
    'result : 0'    # 得到【第三次】生成的结果 0
    func_2()
    'result : 0'    # 得到【第四次】生成的结果 0
    func_2()
    'result : 0'    # 得到【第五次】生成的结果 0
    ```

- 所有对于 func_2() 的调用， 得到的结果都是一样的。

# 4. yield 与 return 在 scrapy 中的不同应用

- 事实上，对于不需要 for 循环的场景， yield 和 return 是一样的，例如：
    ```
    url = "xxx"
    yield scrapy.Request(url=url, callback=self.xxx)
    return scrapy.Request(url=url, callback=self.xxx)     # 此处使用 return 是一样
    
    
    item = XXX()
    item["xxx"] = response.xpath("xxx").extract_first()
    yield item 
    return item                                           #此处使用 return 是一样
    ```
- 但 **对于在 for 循环中的场景， yield 和 return 绝对不能混用**，例如：
    ```
    for i in range(10):
        url = f"xxx?page={i}"
        yield scrapy.Request(url=url, callback=self.xxx)
        # 此处使用 return 会导致漏抓，只抓了第一页就退出了
        # return scrapy.Request(url=url, callback=self.xxx)     
    
    results = response.xpath("xxx").extract()
    for result in results:
        item = XXX()
        item["xxx"] = result["xxx"]
        yield item 
        #此处使用 return 会导致结果漏传，只传递一条结果给 pipeline
       #  return item                  
    ```
- 所以，**yield 和 return 【在 for 循环场景中不能混用】**！！！
>再说一遍，scrapy 爬虫中，如果在 for 循环中使用 return 替代 yield，会导致【漏抓】，因为此时只返回第一个对象， for 循环就 return 结束了！

