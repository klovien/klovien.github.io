---
layout:     post
title:      安装 scrapy-deltafetch 并处理安装 BerkelyDB、bdsdb4 异常问题
subtitle:   处理Command "python setup.py egg_info" failed ...报错
date:       2022-01-26
author:     dex0423
header-img: img/the-first.png
catalog: false
tags:
    - scrapy
---



1.前言
===
- scrapy-deltafetch，是一个用于解决爬虫去重问题的第三方插件。
- scrapy-deltafetch通过Berkeley DB来记录爬虫每次爬取收集的request和item，当重复执行爬虫时只爬取新的item，从而实现爬虫的增量爬取。
- 在安装scrapy-deltafetch插件时，需要先行安装Berkeley DB 和 bsddb4，期间遇到很棘手的问题，解决花去了一天时间，这里和大家分享一下解决办法。

**注意：网上的其他解决办法我都试过，全都不可以用。下面的解决办法是自己摸索找到的解决方案，亲测成功。**


2.常见问题描述
===

- `pip3 install scrapy-deltafetch` 安装 scrapy-deltafetch 报 `Command "python setup.py egg_info" failed ...` 错误

- 报错内容

>*Command "python setup.py egg_info" failed with error code 1 in /private/var/folders/nw/0lc60c214hj4rcyjm_c4fywh0000gn/T/pip-install-no_thc86/bsddb3/
You are using pip version 10.0.1, however version 18.1 is available.
You should consider upgrading via the 'pip install --upgrade pip' command.*



- 安装 bsddb3 时报 `python setup.py egg_info Check the logs for full command output.` 错误

- 报错内容

>    ERROR: Command errored out with exit status 1:
     command: /usr/local/opt/python/bin/python3.7 -c 'import sys, setuptools, tokenize; sys.argv[0] = '"'"'/private/var/folders/ch/6mh0_xrs05jgmw7778l4jr500000gn/T/pip-install-gmeb16ru/bsddb3/setup.py'"'"'; __file__='"'"'/private/var/folders/ch/6mh0_xrs05jgmw7778l4jr500000gn/T/pip-install-gmeb16ru/bsddb3/setup.py'"'"';f=getattr(tokenize, '"'"'open'"'"', open)(__file__);code=f.read().replace('"'"'\r\n'"'"', '"'"'\n'"'"');f.close();exec(compile(code, __file__, '"'"'exec'"'"'))' egg_info --egg-base /private/var/folders/ch/6mh0_xrs05jgmw7778l4jr500000gn/T/pip-install-gmeb16ru/bsddb3/pip-egg-info
cwd: /private/var/folders/ch/6mh0_xrs05jgmw7778l4jr500000gn/T/pip-install-gmeb16ru/bsddb3/
Complete output (27 lines):
Found Berkeley DB 6.2 installation.
      include files in /usr/local/BerkeleyDB.6.2/include
      library files in /usr/local/BerkeleyDB.6.2/lib
      library name is libdb-6.2
    Detected Berkeley DB version 6.2 from db.h
    ******* COMPILATION ABORTED *******
    You are linking a Berkeley DB version licensed under AGPL3 or have a commercial license.
    AGPL3 is a strong copyleft license and derivative works must be equivalently licensed.
    You have two choices:
      1. If your code is AGPL3 or you have a commercial Berkeley DB license from Oracle, please, define the environment variable 'YES_I_HAVE_THE_RIGHT_TO_USE_THIS_BERKELEY_DB_VERSION' to any value, and try to install this python library again.
      2. In any other case, you have to link to a previous version of Berkeley DB. Remove Berlekey DB version 6.x and let this python library try to locate an older version of the Berkeley DB library in your system. Alternatively, you can define the environment variable 'BERKELEYDB_DIR', or 'BERKELEYDB_INCDIR' and 'BERKELEYDB_LIBDIR', with the path of the Berkeley DB you want to use and try to install this python library again.
    Sorry for the inconvenience. I am trying to protect you.
    More details:
        https://forums.oracle.com/message/11184885
        http://lists.debian.org/debian-legal/2013/07/
    ******* COMPILATION ABORTED *******
    ---------------------------------------- 
>ERROR: Command errored out with exit status 1: python setup.py egg_info Check the logs for full command output.



3. 正确安装方法
====


3.1. 安装berkeley-db4
---

```python
brew install berkeley-db4
```

![image.png](https://upload-images.jianshu.io/upload_images/14502986-9a1f2380c95ce79c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


3.2. 设置环境变量
---

```
echo 'export PATH="/usr/local/opt/berkeley-db@4/bin:$PATH"' >> ~/.bash_profile
export LDFLAGS="-L/usr/local/opt/berkeley-db@4/lib"
export CPPFLAGS="-I/usr/local/opt/berkeley-db@4/include"
```


3.3. 安装bsddb3
---

```
YES_I_HAVE_THE_RIGHT_TO_USE_THIS_BERKELEY_DB_VERSION=1 BERKELEYDB_DIR=/usr/local/opt/berkeley-db@4/ pip3 install bsddb3
```
**说明**：很多人遇到问题都是出现在这一步，如果直接 pip3 install bsddb3 是不能安装成功的，这是因为再执行安装命令的时候，需要增加两个执行条件。
- 执行条件包括：

-- 条件一：***将"YES_I_HAVE_THE_RIGHT_TO_USE_THIS_BERKELEY_DB_VERSION" 添加到命令行中、增加对于 Berkely DB 的调用权限；***

-- 条件二：  ***将 Berkely DB 的文件路径 "/usr/local/opt/berkeley-db@4" 赋值给命令中的 BERKELEYDB_DIR；***

![image.png](https://upload-images.jianshu.io/upload_images/14502986-287eff9ee3437110.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


3.4. 安装 scrapy-deltafetch
---

```
pip3 install scrapy-deltafetch
```
![image.png](https://upload-images.jianshu.io/upload_images/14502986-c109442f6bb4634e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


4.补充说明
===

**注意：我使用的电脑为Mac，Windows环境下是情况不清楚。**
