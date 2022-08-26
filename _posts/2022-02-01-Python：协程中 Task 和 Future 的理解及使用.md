---
layout:     post
title:      Python：协程中 Task 和 Future 的理解及使用
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 1. Task 概念及用法
- Task，是 python 中与事件循环进行交互的一种主要方式。
  创建 Task，意思就是把协程封装成 Task 实例，并追踪协程的 **运行 / 完成状态**，用于未来获取协程的结果。
- Task 核心作用：**在事件循环中添加多个并发任务**；
  具体来说，是通过 ```asyncio.create_task()``` 创建 Task，让协程对象加入时事件循环中，等待被调度执行。
>注意：Python 3.7 以后的版本支持 ```asyncio.create_task()```，在此之前的写法为 ```loop.create_task()```，开发过程中需要注意代码写法对不同版本 python 的兼容性。
- 需要指出的是，协程封装为 Task 后不会立马启动，当某个代码 ```await``` 这个 Task 的时候才会被执行。
>当多个 Task 被加入一个 task_list 的时候，添加 Task 的过程中 Task 不会执行，必须要用 ```await asyncio.wait()``` 或 ```await asyncio.gather()``` 将 Task 对象加入事件循环中异步执行。
- 一般在开发中，常用的写法是这样的：
  -- 先创建 ```task_list``` 空列表；
  -- 然后用 ```asyncio.create_task()``` 创建 Task；
  -- 再把 Task 对象加入 ```task_list```；
  -- 最后使用 ```await asyncio.wait``` 或 ```await asyncio.gather``` 将 Task 对象加入事件循环中异步执行。
>注意：创建 Task 对象时，除了可以使用 ```asyncio.create_task()``` 之外，还可以用最低层级的 ```loop.create_task()``` 或 ```asyncio.ensure_future()```，他们都可以用来创建 Task 对象，其中关于 ```ensure_future``` 相关内容本文接下来会一起讲。


- Task 用法代码示例：
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

# 2. Future 概念解读
- 在介绍 Future 之前有两点问题需要先说明：
  1、Future 相较于 Task 属于更底层的概念，在开发过程中用到的并不多，这里介绍 Future 主要是为了加深对于 Task 的理解；
  2、这里指的是 ```asyncio.Future``` 而不是 ```coroutines.futures.Future```，```coroutines.futures.Future``` 常用于 **多进程、多线程实现并发**。
- Future，又称 **未来对象、期程对象**，其本质上是一个容器，用于接受异步执行的结果；
- 我们前面讲的 **Task 是继承自 Future** ！
- Furture 对象内部封装了一个 ```_state```，这个 ```_state``` 维护着四种状态：**Pending**、**Running**、**Done**，**Cancelled**，如果变成 ```Done``` 完成，就不再等待，而是往后执行，这四种状态的存在其实类似与进程的 运行态、就绪态、阻塞态，事件循环凭借着四种状态对 Future\协程对象 进行调度。
- 在开发中，如果直接创建 Future 需要使用 ```asyncio.ensure_future()``` 函数，下面是 ```ensure_future``` 函数的源码，仔细阅读源码我们会发现，```ensure_future``` 函数最后返回的一定是一个 ```awaitable``` 对象，即满足 Awaitable 协议。
>正因为 ```ensure_future``` 函数最后返回的一定是一个 ```awaitable``` 对象，所以才保证了继承自 Future 的 Task 是 ```awaitable``` 的。
同时，协程对象一位内无法自己执行，需要将其注册到事件循环中转变为一个 Task 对象才会被执行，所以协程对象一定 ```awaitable``` 的。

- 一般只有在一定要确保需要创建一个 ```awaitable``` 对象的时候，才会使用 ```ensure_future``` 函数。
```
def ensure_future(coro_or_future, *, loop=None):
    """Wrap a coroutine or an awaitable in a future.

    If the argument is a Future, it is returned directly.
    """
    if coroutines.iscoroutine(coro_or_future):
        if loop is None:
            loop = events.get_event_loop()
        task = loop.create_task(coro_or_future)
        if task._source_traceback:
            del task._source_traceback[-1]
        return task
    elif futures.isfuture(coro_or_future):
        if loop is not None and loop is not futures._get_loop(coro_or_future):
            raise ValueError('loop argument must agree with Future')
        return coro_or_future
    elif inspect.isawaitable(coro_or_future):
        return ensure_future(_wrap_awaitable(coro_or_future), loop=loop)
    else:
        raise TypeError('An asyncio.Future, a coroutine or an awaitable is '
                        'required')

@coroutine
def _wrap_awaitable(awaitable):
    """Helper for asyncio.ensure_future().

    Wraps awaitable (an object with __await__) into a coroutine
    that will later be wrapped in a Task by ensure_future().
    """
    return (yield from awaitable.__await__())
```
- 下面是 Future 类的源码，感兴趣的小伙伴可以研究一下。
```
class Future:
    """This class is *almost* compatible with concurrent.futures.Future.

    Differences:

    - This class is not thread-safe.

    - result() and exception() do not take a timeout argument and
      raise an exception when the future isn't done yet.

    - Callbacks registered with add_done_callback() are always called
      via the event loop's call_soon().

    - This class is not compatible with the wait() and as_completed()
      methods in the concurrent.futures package.

    (In Python 3.4 or later we may be able to unify the implementations.)
    """

    # Class variables serving as defaults for instance variables.
    _state = _PENDING
    _result = None
    _exception = None
    _loop = None
    _source_traceback = None

    # This field is used for a dual purpose:
    # - Its presence is a marker to declare that a class implements
    #   the Future protocol (i.e. is intended to be duck-type compatible).
    #   The value must also be not-None, to enable a subclass to declare
    #   that it is not compatible by setting this to None.
    # - It is set by __iter__() below so that Task._step() can tell
    #   the difference between
    #   `await Future()` or`yield from Future()` (correct) vs.
    #   `yield Future()` (incorrect).
    _asyncio_future_blocking = False

    __log_traceback = False

    def __init__(self, *, loop=None):
        """Initialize the future.

        The optional event_loop argument allows explicitly setting the event
        loop object used by the future. If it's not provided, the future uses
        the default event loop.
        """
        if loop is None:
            self._loop = events.get_event_loop()
        else:
            self._loop = loop
        self._callbacks = []
        if self._loop.get_debug():
            self._source_traceback = format_helpers.extract_stack(
                sys._getframe(1))

    _repr_info = base_futures._future_repr_info

    def __repr__(self):
        return '<{} {}>'.format(self.__class__.__name__,
                                ' '.join(self._repr_info()))

    def __del__(self):
        if not self.__log_traceback:
            # set_exception() was not called, or result() or exception()
            # has consumed the exception
            return
        exc = self._exception
        context = {
            'message':
                f'{self.__class__.__name__} exception was never retrieved',
            'exception': exc,
            'future': self,
        }
        if self._source_traceback:
            context['source_traceback'] = self._source_traceback
        self._loop.call_exception_handler(context)

    @property
    def _log_traceback(self):
        return self.__log_traceback

    @_log_traceback.setter
    def _log_traceback(self, val):
        if bool(val):
            raise ValueError('_log_traceback can only be set to False')
        self.__log_traceback = False

    def get_loop(self):
        """Return the event loop the Future is bound to."""
        return self._loop

    def cancel(self):
        """Cancel the future and schedule callbacks.

        If the future is already done or cancelled, return False.  Otherwise,
        change the future's state to cancelled, schedule the callbacks and
        return True.
        """
        self.__log_traceback = False
        if self._state != _PENDING:
            return False
        self._state = _CANCELLED
        self.__schedule_callbacks()
        return True

    def __schedule_callbacks(self):
        """Internal: Ask the event loop to call all callbacks.

        The callbacks are scheduled to be called as soon as possible. Also
        clears the callback list.
        """
        callbacks = self._callbacks[:]
        if not callbacks:
            return

        self._callbacks[:] = []
        for callback, ctx in callbacks:
            self._loop.call_soon(callback, self, context=ctx)

    def cancelled(self):
        """Return True if the future was cancelled."""
        return self._state == _CANCELLED

    # Don't implement running(); see http://bugs.python.org/issue18699

    def done(self):
        """Return True if the future is done.

        Done means either that a result / exception are available, or that the
        future was cancelled.
        """
        return self._state != _PENDING

    def result(self):
        """Return the result this future represents.

        If the future has been cancelled, raises CancelledError.  If the
        future's result isn't yet available, raises InvalidStateError.  If
        the future is done and has an exception set, this exception is raised.
        """
        if self._state == _CANCELLED:
            raise CancelledError
        if self._state != _FINISHED:
            raise InvalidStateError('Result is not ready.')
        self.__log_traceback = False
        if self._exception is not None:
            raise self._exception
        return self._result

    def exception(self):
        """Return the exception that was set on this future.

        The exception (or None if no exception was set) is returned only if
        the future is done.  If the future has been cancelled, raises
        CancelledError.  If the future isn't done yet, raises
        InvalidStateError.
        """
        if self._state == _CANCELLED:
            raise CancelledError
        if self._state != _FINISHED:
            raise InvalidStateError('Exception is not set.')
        self.__log_traceback = False
        return self._exception

    def add_done_callback(self, fn, *, context=None):
        """Add a callback to be run when the future becomes done.

        The callback is called with a single argument - the future object. If
        the future is already done when this is called, the callback is
        scheduled with call_soon.
        """
        if self._state != _PENDING:
            self._loop.call_soon(fn, self, context=context)
        else:
            if context is None:
                context = contextvars.copy_context()
            self._callbacks.append((fn, context))

    # New method not in PEP 3148.

    def remove_done_callback(self, fn):
        """Remove all instances of a callback from the "call when done" list.

        Returns the number of callbacks removed.
        """
        filtered_callbacks = [(f, ctx)
                              for (f, ctx) in self._callbacks
                              if f != fn]
        removed_count = len(self._callbacks) - len(filtered_callbacks)
        if removed_count:
            self._callbacks[:] = filtered_callbacks
        return removed_count

    # So-called internal methods (note: no set_running_or_notify_cancel()).

    def set_result(self, result):
        """Mark the future done and set its result.

        If the future is already done when this method is called, raises
        InvalidStateError.
        """
        if self._state != _PENDING:
            raise InvalidStateError('{}: {!r}'.format(self._state, self))
        self._result = result
        self._state = _FINISHED
        self.__schedule_callbacks()

    def set_exception(self, exception):
        """Mark the future done and set an exception.

        If the future is already done when this method is called, raises
        InvalidStateError.
        """
        if self._state != _PENDING:
            raise InvalidStateError('{}: {!r}'.format(self._state, self))
        if isinstance(exception, type):
            exception = exception()
        if type(exception) is StopIteration:
            raise TypeError("StopIteration interacts badly with generators "
                            "and cannot be raised into a Future")
        self._exception = exception
        self._state = _FINISHED
        self.__schedule_callbacks()
        self.__log_traceback = True

    def __await__(self):
        if not self.done():
            self._asyncio_future_blocking = True
            yield self  # This tells Task to wait for completion.
        if not self.done():
            raise RuntimeError("await wasn't used with future")
        return self.result()  # May raise too.

    __iter__ = __await__  # make compatible with 'yield from'.
```

