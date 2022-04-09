---
layout:     post
title:      JS逆向：破解企查查无限 debugger 反爬策略
subtitle:   Fiddler + AutoResponder 篡改 js 代码
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - JS逆向
---


# 1. 前言
今天在搞企查查的时候，发现企查查开启了无限 debugger，右键检查打开调试窗口后，无论在企查查页面进行任何操作，都会进入 debugger 调试状态。
如下图：

![]({{site.baseurl}}/img-post/debugger-1.png)

按照常规的思路，找到 debugger 关键字，之后禁用调试或者添加 false 条件，不执行 debugger 就可以了，但是当我全局搜索 debugger 关键字的时候，却发现 js 文件中的 debugger 竟然有三千多个，瞬间心里三千只草泥马奔腾而过。
![]({{site.baseurl}}/img-post/debugger-2.png)

此时，我尝试分析 js 文件，搞清楚 debugger 函数的执行逻辑，但是经过分析发现，debugger 的执行逻辑全部封装在一个匿名函数里，当请求返回的时候函数会自动执行，不要监听任何事件即可完成 debugger 调用。
![]({{site.baseurl}}/img-post/debugger-2-1.png)

# 2. Fiddler 拦截思路
这个时候只能祭出大杀器 fiddler，采用中间人攻击的方式篡改 js 文件。
这里再简单介绍一下 Fiddler，可能很多人知道 Fiddler 是一个抓包工具，但其实 Fiddler 的作用远不止于此。
Fiddler 是一个 http 协议调试工具，Fiddler 数据抓包软件能够记录并检查所有你的电脑和互联网之间的http通讯，并可以设置断点、查看所有的“进出”Fiddler的数据，还可以自定义修改 cookie、html、js、css 等文件。
这里我将拦截企查查返回的 js 文件，并用篡改后的 js 文件取而代之，从而实现规避无限 debugger 的目的。

# 3. 获取 js 路径
如下图所示，获取 js 文件的 url 路径。
![]({{site.baseurl}}/img-post/debugger-3.png)

# 4. 篡改 js 文件
这里，首先将 js 文件保存到本地，之后将 ```    debugger```字符串替换成```    false```。
>注意：**debugger 前面有空格**，替换时最好带上空格，因为有些位置也出现了 ```debugger``` 字符串，但是并不是我们需要替换的函数。

![]({{site.baseurl}}/img-post/debugger-4.png)

# 5. Fiddler 拦截
在 ```AutoResponder```中添加规则，如下图所示。
![]({{site.baseurl}}/img-post/debugger-5.png)

# 6. 重新请求
此时，无限 debugger 已经不再出现。
![]({{site.baseurl}}/img-post/debugger-6.png)
