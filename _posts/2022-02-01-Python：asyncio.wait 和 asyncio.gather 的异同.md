---
layout:     post
title:      Python：asyncio.wait 和 asyncio.gather 的异同
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. 异同点综述
- **相同**：
  - 从功能上看，```asyncio.wait``` 和 ```asyncio.gather``` 实现的效果是相同的，都是把所有 Task 任务结果收集起来。

- **不同**：
  - ```asyncio.wait``` 会返回两个值：```done``` 和 ```pending```，```done``` 为已完成的协程 ```Task```，```pending``` 为超时未完成的协程 ```Task```，需通过 ```future.result``` 调用 ```Task``` 的 ```result```；
  - 而```asyncio.gather``` 返回的是所有已完成 ```Task``` 的 ```result```，不需要再进行调用或其他操作，就可以得到全部结果。

# 2. asyncio.wait 用法：
最常见的写法是：```await asyncio.wait(task_list)```。

```
import asyncio
import arrow


def current_time():
    '''
    获取当前时间
    :return:
    '''
    cur_time = arrow.now().to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss')
    return cur_time


async def func(sleep_time):
    func_name_suffix = sleep_time        # 使用 sleep_time（函数 I/O 等待时长）作为函数名后缀，以区分任务对象
    print(f"[{current_time()}] 执行异步函数 {func.__name__}-{func_name_suffix}")
    await asyncio.sleep(sleep_time)
    print(f"[{current_time()}] 函数 {func.__name__}-{func_name_suffix} 执行完毕")
    return f"【[{current_time()}] 得到函数 {func.__name__}-{func_name_suffix} 执行结果】"


async def run():
    task_list = []
    for i in range(5):
        task = asyncio.create_task(async_func(i))
        task_list.append(task)

    done, pending = await asyncio.wait(task_list, timeout=None)
    for done_task in done:
        print((f"[{current_time()}] 得到执行结果 {done_task.result()}"))

def main():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run())


if __name__ == '__main__':
    main()
```
代码执行结果如下：
```json
[2020-11-03 22:45:53] 执行异步函数 func-0
[2020-11-03 22:45:53] 执行异步函数 func-1
[2020-11-03 22:45:53] 执行异步函数 func-2
[2020-11-03 22:45:53] 执行异步函数 func-3
[2020-11-03 22:45:53] 执行异步函数 func-4
[2020-11-03 22:45:53] 函数 func-0 执行完毕
[2020-11-03 22:45:54] 函数 func-1 执行完毕
[2020-11-03 22:45:55] 函数 func-2 执行完毕
[2020-11-03 22:45:56] 函数 func-3 执行完毕
[2020-11-03 22:45:57] 函数 func-4 执行完毕
[2020-11-03 22:45:57] 得到执行结果 【[2020-11-03 22:45:57] 得到函数 func-4 执行结果】
[2020-11-03 22:45:57] 得到执行结果 【[2020-11-03 22:45:55] 得到函数 func-2 执行结果】
[2020-11-03 22:45:57] 得到执行结果 【[2020-11-03 22:45:53] 得到函数 func-0 执行结果】
[2020-11-03 22:45:57] 得到执行结果 【[2020-11-03 22:45:56] 得到函数 func-3 执行结果】
[2020-11-03 22:45:57] 得到执行结果 【[2020-11-03 22:45:54] 得到函数 func-1 执行结果】
```
# 3. asyncio.gather 用法：
最常见的用法是：```await asyncio.gather(*task_list)```，注意这里 ```task_list``` 前面有一个 ```*```。
```
import asyncio
import arrow


def current_time():
    '''
    获取当前时间
    :return:
    '''
    cur_time = arrow.now().to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss')
    return cur_time


async def func(sleep_time):
    func_name_suffix = sleep_time     # 使用 sleep_time（函数 I/O 等待时长）作为函数名后缀，以区分任务对象
    print(f"[{current_time()}] 执行异步函数 {func.__name__}-{func_name_suffix}")
    await asyncio.sleep(sleep_time)
    print(f"[{current_time()}] 函数 {func.__name__}-{func_name_suffix} 执行完毕")
    return f"【[{current_time()}] 得到函数 {func.__name__}-{func_name_suffix} 执行结果】"


async def run():
    task_list = []
    for i in range(5):
        task = asyncio.create_task(func(i))
        task_list.append(task)
    results = await asyncio.gather(*task_list)
    for result in results:
        print((f"[{current_time()}] 得到执行结果 {result}"))


def main():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(run())


if __name__ == '__main__':
    main()

```
代码执行结果如下：
```json
[2020-11-03 22:35:54] 执行异步函数 func-0
[2020-11-03 22:35:54] 执行异步函数 func-1
[2020-11-03 22:35:54] 执行异步函数 func-2
[2020-11-03 22:35:54] 执行异步函数 func-3
[2020-11-03 22:35:54] 执行异步函数 func-4
[2020-11-03 22:35:54] 函数 func-0 执行完毕
[2020-11-03 22:35:55] 函数 func-1 执行完毕
[2020-11-03 22:35:56] 函数 func-2 执行完毕
[2020-11-03 22:35:57] 函数 func-3 执行完毕
[2020-11-03 22:35:58] 函数 func-4 执行完毕
[2020-11-03 22:35:58] 得到执行结果 【[2020-11-03 22:35:54] 得到函数 func-0 执行结果】
[2020-11-03 22:35:58] 得到执行结果 【[2020-11-03 22:35:55] 得到函数 func-1 执行结果】
[2020-11-03 22:35:58] 得到执行结果 【[2020-11-03 22:35:56] 得到函数 func-2 执行结果】
[2020-11-03 22:35:58] 得到执行结果 【[2020-11-03 22:35:57] 得到函数 func-3 执行结果】
[2020-11-03 22:35:58] 得到执行结果 【[2020-11-03 22:35:58] 得到函数 func-4 执行结果】
```