---
layout:     post
title:      Python：asyncio 概念和用法详解
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 0. 前言
- 在执行一些 IO 密集型任务的时候，程序常常会因为等待 IO 而阻塞。比如在网络爬虫中，如果我们使用 requests 库来进行请求的话，如果网站响应速度过慢，程序一直在等待网站响应，最后导致其爬取效率是非常非常低的。
- 为了解决这类问题，本文就来探讨一下 Python 中异步协程来加速的方法，此种方法对于 IO 密集型任务非常有效。如将其应用到网络爬虫中，爬取效率甚至可以成百倍地提升；
# 1. 理解协程
#### 1.1. 协程定义
- 协程，英文叫做 Coroutine，又称微线程，纤程，协程是一种用户态的轻量级线程；
- 协程拥有自己的寄存器上下文和栈。协程调度切换时，将寄存器上下文和栈保存到其他地方，在切回来的时候，恢复先前保存的寄存器上下文和栈。因此协程能保留上一次调用时的状态，即所有局部状态的一个特定组合，每次过程重入时，就相当于进入上一次调用的状态。
- 协程本质上是个单进程，协程相对于多进程来说，无需线程上下文切换的开销，无需原子操作锁定及同步的开销，编程模型也非常简单；
- 我们可以使用协程来实现异步操作，比如在网络爬虫场景下，我们发出一个请求之后，需要等待一定的时间才能得到响应，但其实在这个等待过程中，程序可以干许多其他的事情，等到响应得到之后才切换回来继续处理，这样可以充分利用 CPU 和其他资源，这就是异步协程的优势。
- 其实在其他语言中，协程的其实是意义不大的多线程即可已解决I/O的问题，但是在python因为他有GIL（Global Interpreter Lock 全局解释器锁 ）在同一时间只有一个线程在工作，所以：如果一个线程里面I/O操作特别多，协程就比较适用;
#### 1.2. 协程的优势
- 无需线程上下文切换的开销；
- 无需原子操作锁定及同步的开销；
- 方便切换控制流，简化编程模型
- 高并发+高扩展性+低成本：一个CPU支持上万的协程都不是问题。所以很适合用于高并发处理。
#### 1.3. 协程缺点
- 无法利用多核资源：协程的本质是个单线程，它不能同时将 单个CPU 的多个核用上，协程需要和进程配合才能运行在多CPU上.当然我们日常所编写的绝大部分应用都没有这个必要，除非是cpu密集型应用；
- 进行阻塞（Blocking）操作（如IO时）会阻塞掉整个程序

# 3. 协程关键字
#### 3.1. async
- **async def func(): ...**
  - async 作为一个关键字放在函数的前面，表示该函数是一个异步函数；
  - async 定义后的异步函数，在执行时不会阻塞后面代码的执行；
  - 异步函数 func() 同样可以单独、或者在 class 类中被调用，与普通函数的调用方法是一样的；
- **async for**
- **async with**
- **task**
  - 任务，它是对协程对象的进一步封装，包含了任务的各个状态；
- **future**
  - 代表将来执行或没有执行的任务的结果，实际上和 task 没有本质区别。
- **gather**
  - gather 可以将任务分组

#### 3.2. await
- await 用来用来声明程序挂起，比如异步程序执行到某一步时需要等待的时间很长，就将此挂起，去执行其他的异步程序；
- await 后面只能跟异步程序或有 __ await __ 属性的对象，否则会报错；
- 假设有两个异步函数 async a，async b，a 中的某一步有 await，当程序碰到关键字 await b() 后，异步程序挂起后去执行另一个异步 b 程序，就是从函数内部跳出去执行其他函数，当挂起条件消失后，不管 b 是否执行完，要马上从 b 程序中跳出来，回到原程序执行原来的操作；
- 如果 await 后面跟的 b 函数不是异步函数，那么操作就只能等b执行完再返回，无法在 b 执行的过程中返回。如果要在 b 执行完才返回，也就不需要用 await 关键字了，直接调用 b 函数就行，所以这就需要 await 后面跟的是异步函数了；
- 在一个异步函数中，可以不止一次挂起，也就是可以用多个 await；
- **await 的用法和 yield from 用法类似**，但是 await 后面只能跟 Awaitable 的对象（实现了 __await __ 魔法方法），而 yield from 后面可以跟生成器、协程等；
- await asyncio.sleep()
  - 需要 sleep 的时候用 await asyncio.sleep(xx)， time.sleep(xx) 是同步阻塞接口，不能用于协程中；
#### 3.3. asyncio
- **asyncio.get_event_loop()**

#### 3.4. loop 事件循环
- **loop.run_until_complete(asyncio.wait(tasks))**

# 4. 协程用法
# 4.1.  同步执行函数示例
- 下面的代码是常规的同步执行代码，用于和异步执行方法作对比；
```
import time


def timer(func):
    """
    定义一个装饰器，用于统计程序执行所花费的时间
    :param func: 被统计的函数
    :return:
    """
    def call_func(*args, **kwargs):
        print("计时开始")
        start_time = time.time()
        try:
            func(*args, **kwargs)
        except KeyboardInterrupt:
            pass
        finally:
            end_time = time.time()
            total_time = end_time - start_time
            print("计时结束")
            print(f"程序用时{int(total_time // 60)}分{total_time % 60:.2f}秒")
    return call_func


def task(i):
    print(f'task {i} start : {time.strftime("%X", time.localtime())}')
    time.sleep(1)
    print(f'task {i} finish : {time.strftime("%X", time.localtime())}')


@timer
def main():
    for i in range(5):
        task(i)


if __name__ == '__main__':
    main()
```
- 执行结果
```
计时开始
task 0 start : 10:59:58
task 0 finish : 10:59:59
task 1 start : 10:59:59
task 1 finish : 11:00:00
task 2 start : 11:00:00
task 2 finish : 11:00:01
task 3 start : 11:00:01
task 3 finish : 11:00:02
task 4 start : 11:00:02
task 4 finish : 11:00:03
计时结束
程序用时0分5.03秒
```
#### 4.2. asyncio 用法示例
- 下面的代码展示了 asyncio 的基础用法
```
import time
import asyncio


def timer(func):
    """
    定义一个装饰器，用于统计程序执行所花费的时间
    :param func: 被统计的函数
    :return:
    """
    def call_func(*args, **kwargs):
        print("计时开始")
        start_time = time.time()
        try:
            func(*args, **kwargs)
        except KeyboardInterrupt:
            pass
        finally:
            end_time = time.time()
            total_time = end_time - start_time
            print("计时结束")
            print(f"程序用时{int(total_time // 60)}分{total_time % 60:.2f}秒")
    return call_func


async def task(i):
    print(f'task {i} start : {time.strftime("%X", time.localtime())}')
    await asyncio.sleep(1)    # 异步函数中 需要 sleep 的时候用 await asyncio.sleep()
    print(f'task {i} finish : {time.strftime("%X", time.localtime())}')


@timer
def run():
    loop = asyncio.get_event_loop()
    # 这个 tasks 可以是不同的协程
    tasks = [task(i) for i in range(5)]
    # asyncio.wait()函数会接收一个可迭代对象
    loop.run_until_complete(asyncio.wait(tasks))


if __name__ =='__main__':
    run()
```
- 运行结果
  - 通过和前面的对比，我们发现 asyncio 所花的时间只有同步执行的 1/5，也可以说执行的效率要提升了 5 倍；
```
计时开始
task 4 start : 11:11:09
task 2 start : 11:11:09
task 0 start : 11:11:09
task 1 start : 11:11:09
task 3 start : 11:11:09
task 4 finish : 11:11:10
task 0 finish : 11:11:10
task 3 finish : 11:11:10
task 2 finish : 11:11:10
task 1 finish : 11:11:10
计时结束
程序用时0分1.00秒
```

- **使用 asyncio 注意事项**
>注意：做数据库驱动、网络请求驱动，一定要有对应的异步库，不能使用传统的阻塞接口；
例如，pymysql 不能满足 asyncio 异步需求，因为 pymysql 接口是阻塞的，而 asyncio 是单线程的，只要一个地方阻塞其他的都会被阻塞；


#### 3.4. asyncio 用法示例
- run_until_complete() 用法
```

import asyncio
import time


async def parse_list(page_url):
    print(f"start parse list : {page_url}")
    # 解析网页的过程此处省略
    await asyncio.sleep(2)     # 注意：不能使用 time.sleep()，因为 time.sleep() 是同步阻塞接口，不能用于异步编程
    print(f"parse list finished : {page_url}")


def main():
    start_time = time.time()
    loop = asyncio.get_event_loop()
    page_urls = [f"https://static1.scrape.cuiqingcai.com/page/{page}" for page in range(0, 11)]
    tasks = [parse_list(page_url) for page_url in page_urls]
    loop.run_until_complete(asyncio.wait(tasks))
    end_time = time.time()
    print("SPEND TIME : {}".format(end_time-start_time))


if __name__ == '__main__':
    main()

```
- run_forever() & task.cancel() 用法；
```

import asyncio
import time
from concurrent.futures import ThreadPoolExecutor


async def parse_list(page_url):
    print(f"start parse list : {page_url}")
    # 解析网页的过程此处省略
    await asyncio.sleep(2)     # 注意：不能使用 time.sleep()，因为 time.sleep() 是同步阻塞接口，不能用于异步编程
    print(f"parse list finished : {page_url}")
    parse_list_result = f"{page_url} result"
    return parse_list_result


def main():
    start_time = time.time()
    loop = asyncio.get_event_loop()

    tasks_1 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(0, 11)]
    tasks_2 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(11, 21)]
    tasks_3 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(21, 31)]

    tasks = tasks_1 + tasks_2 + tasks_3
    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(asyncio.wait(tasks))
    except KeyboardInterrupt as e:
        all_tasks = asyncio.Task.all_tasks()
        for task in all_tasks:
            task.cancel()
            print(f"task {task} called")
        loop.stop()
        loop.run_forever()
    finally:
        loop.close()


if __name__ == '__main__':
    main()

```
- ask.add_done_callback() & task.result() 用法
  - task.add_done_callback(call_back_func)，添加回调函数；
  - task.result()，获取协程的返回值；
```
import asyncio
import time


async def parse_list(page_url):
    print(f"start parse list : {page_url}")
    # 解析网页的过程此处省略
    await asyncio.sleep(2)     # 注意：不能使用 time.sleep()，因为 time.sleep() 是同步阻塞接口，不能用于异步编程
    print(f"parse list finished : {page_url}")
    parse_list_result = f"{page_url} result"
    return parse_list_result


def call_back_func(object):
    parse_list_result = object.result()                 # object.result()，用于获取 parse_list 的返回值
    print(f"call back {parse_list_result}")


def main():
    start_time = time.time()
    loop = asyncio.get_event_loop()

    page = 1
    page_url = f"https://static1.scrape.cuiqingcai.com/page/{page}"
    coroutine = parse_list(page_url)
    task = loop.create_task(coroutine)
    # task = asyncio.ensure_future(coroutine)          # 这种写法和上面一句作用是一样的

    task.add_done_callback(call_back_func)             # task.add_done_callback 用于添加回调函数，表示函数执行完毕后调用此函数
    loop.run_until_complete(task)
    print(f"get parse list result : {task.result()}")
    end_time = time.time()
    print("SPEND TIME : {}".format(end_time-start_time))

if __name__ == '__main__':
    main()

```

- gather() 用法；
```
import asyncio
import time
import concurrent


async def parse_list(page_url):
    print(f"start parse list : {page_url}")
    # 解析网页的过程此处省略
    await asyncio.sleep(2)     # 注意：不能使用 time.sleep()，因为 time.sleep() 是同步阻塞接口，不能用于异步编程
    print(f"parse list finished : {page_url}")
    parse_list_result = f"{page_url} result"
    return parse_list_result


def main():
    start_time = time.time()
    loop = asyncio.get_event_loop()

    tasks_1 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(0, 11)]
    tasks_2 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(11, 21)]
    tasks_3 = [parse_list(f"https://static1.scrape.cuiqingcai.com/page/{page}") for page in range(21, 31)]

    tasks_1 = asyncio.gather(*tasks_1)
    tasks_2 = asyncio.gather(*tasks_2)
    tasks_3 = asyncio.gather(*tasks_3)

    tasks_2.cancel()                                                        # 取消任务

    # loop.run_until_complete(asyncio.gather(tasks_1, tasks_2, tasks_3))    # 直接 run 执行 task_2 时会报错，因为已经被取消

    try:
        loop.run_until_complete(asyncio.gather(tasks_1, tasks_2, tasks_3))
    except concurrent.futures._base.CancelledError as e:                    # task_2 执行报错会在这里被捕获
        print("task cancell")

    end_time = time.time()
    print("SPEND TIME : {} ".format(end_time-start_time))


if __name__ == '__main__':
    main()

```
-  使用 ThreadPoolExecutor 在协程中集成阻塞 I/O；
   - 在协程中集成阻塞 I/O，必须要用到 **loop.run_in_executor(None, func, args)**，其中 None 是用于存放线程（也可以是进程）的 ppol 池，func 是阻塞的函数，args 是阻塞函数的参数；
```

import asyncio
from concurrent.futures import ThreadPoolExecutor
import socket
from urllib.parse import urlparse


def parse_list(page_url):
    print(f"start parse list : {page_url}")
    # 解析网页的过程此处省略
    # asyncio.sleep(2)      # asyncio.sleep 用于异步编程，此处为演示阻塞接口，所以没有使用
    time.sleep(2)           # 注意：此处故意使用 time.sleep()，因为 time.sleep() 是同步阻塞接口，
    print(f"parse list finished : {page_url}")
    parse_list_result = f"{page_url} result"
    return parse_list_result


def main():
    start_time = time.time()
    loop = asyncio.get_event_loop()
    executor = ThreadPoolExecutor()
    tasks = []
    for page in range(1, 31):
        page_url = f"http://shop.projectsedu.com/goods/{page}/"
        task = loop.run_in_executor(executor, parse_list, page_url)
        tasks.append(task)
    loop.run_until_complete(asyncio.wait(tasks))
    end_time = time.time()
    print("SPEND TIME : {}".format(end_time-start_time))


if __name__ == '__main__':
    main()

```
- asyncio 模拟 http 请求；
```

# 使用多线程：在携程中集成阻塞io
import asyncio
import socket
from urllib.parse import urlparse


async def get_url(url):
    #通过socket请求html
    url = urlparse(url)
    host = url.netloc
    path = url.path
    if path == "":
        path = "/"

    #建立socket连接
    reader, writer = await asyncio.open_connection(host,80)
    writer.write("GET {} HTTP/1.1\r\nHost:{}\r\nConnection:close\r\n\r\n".format(path, host).encode("utf8"))
    all_lines = []
    async for raw_line in reader:
        data = raw_line.decode("utf8")
        all_lines.append(data)
    html = "\n".join(all_lines)
    return html

async def main():
    tasks = []
    for url in range(20):
        url = "http://shop.projectsedu.com/goods/{}/".format(url)
        tasks.append(asyncio.ensure_future(get_url(url)))
    for task in asyncio.as_completed(tasks):
        result = await task
        print(result)
#
if __name__ == "__main__":
    import time
    start_time = time.time()
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
    print('last time:{}'.format(time.time()-start_time))

```
- 使用 Semaphore 信号量同步机制限制并发量；
```
from aiohttp import ClientSession 
import asyncio

######################
#  限制协程并发量
######################
async def hello(sem, url):
    async with sem:
        async with ClientSession() as session:
            async with session.get(f'http://localhost:8080/{url}') as response:
                r = await response.read()
                print(r)
                await asyncio.sleep(1)


def main():
    loop = asyncio.get_event_loop()
    tasks = []
    sem = asyncio.Semaphore(5)  # this 
    for i in range(100000):
        task = asyncio.ensure_future(hello(sem, i))
        tasks.append(task)
    

    feature = asyncio.ensure_future(asyncio.gather(*tasks))
    loop.run_until_complete(feature)

if __name__ == "__main__":
    main()
```
- asyncio 的 Semaphore 和 aiohttp 限制并发数量；
```
import time
from aiohttp import ClientSession
import asyncio


URL = 'http://httpbin.org/get?a={}'


async def fetch_async(num):
    async with ClientSession() as session:
        async with session.get(URL.format(num)) as response:
            data = await response.json()
            return data['args']['a']


async def print_result(num, semaphore):
    async with semaphore:
        r = await fetch_async(num)
        print(f'fetch({num}) = {r}')


async def main(loop):
    now = time.time()
    semaphore = asyncio.Semaphore(3)
    tasks = [print_result(num, semaphore) for num in range(10)]
    await asyncio.wait(tasks)
    print("总用时", time.time() - now)


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(main(loop))
    finally:
        loop.close()
```
