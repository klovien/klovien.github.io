---
layout:     post
title:      Elasticsearch：Windows 安装 Elasticsearch 及 可视化工具 elasticsear-head
subtitle:   
date:       2022-01-30
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Elasticsearch
---


# 1. 安装 Elasticsearch

- 下载 Elasticsearch；
- 下载地址：[https://www.elastic.co/cn/downloads/elasticsearch](https://www.elastic.co/cn/downloads/elasticsearch)；

- 下载后解压缩；

- 打开压缩后的文件，打开 \bin 目录，
  - 如下：

    ![]({{site.baseurl}}/img-post/es-3.png)
  
- 双击 图中红色框标记的 elasticsearch 文件，系统自动启动命令行窗口，得到下图：
  - 注意这个窗口不要关闭！

    ![]({{site.baseurl}}/img-post/es-4.png)
  
- 如图，在红框中我们看到 127.0.0.1:9200；

- 我们打开浏览器，输入127.0.0.1:9200 回车，得到下图：

  ![]({{site.baseurl}}/img-post/es-5.png)

- 这表示安装成、服务已被启动；

# 2. 安装可视化工具 elasticsear-head

- 参考下载页面：[https://github.com/mobz/elasticsearch-head](https://github.com/mobz/elasticsearch-head)；
    
    ```
    git clone git://github.com/mobz/elasticsearch-head.git
    cd elasticsearch-head
    npm install
    npm run start
    ```
  
- 得到下图：

  ![]({{site.baseurl}}/img-post/es-6.png)

- 打开浏览器，输入 http://localhost:9100，得到下图：

  ![]({{site.baseurl}}/img-post/es-7.png)

- 完成这一步后，我们发下右侧灰色提示 **集群健康值：未连接**，我们点击 【连接】按钮发现并没有任何反应，这是因为 ES 服务 和 elasticsearch-head 之间的通信尚未建立，我们需要进行下一步操作；

# 3. 修改 YML 配置

- 打开 ES 文件包找到 \config 目录下的 elasticsearch.yml 文件，如下图：

  ![]({{site.baseurl}}/img-post/es-8.png)

- 使用文本阅读程序打开 elasticsearch.yml 文件，在最底部添加下面两行内容：

```
http.cors.enabled: true    # 开启跨域访问支持，默认为false
http.cors.allow-origin: "*"   # 跨域访问允许的域名地址
```

- 找到 ES 服务重启后跳出的命令行窗口，ctrl + C 终止服务；
  
- 重新双击 ES 文件夹 \bin 目录下的 elasticsearch，重启 ES 服务；
  
- 刷新 http://localhost:9100 页面，得到下图：

  ![]({{site.baseurl}}/img-post/es-9.png)

- 至此安装全部完成。

# 4. 补充知识：

- 下面是 elasticsearch.yml 文件中配置项的用途和用法，请参考：

    ```
    http.cors.enabled	是否支持跨域，默认为false
    http.cors.allow-origin	当设置允许跨域，默认为*,表示支持所有域名，如果我们只是允许某些网站能访问，那么可以使用正则表达式。比如只允许本地地址。 /https?:\/\/localhost(:[0-9]+)?/
    http.cors.max-age	浏览器发送一个“预检”OPTIONS请求，以确定CORS设置。最大年龄定义多久的结果应该缓存。默认为1728000（20天）
    http.cors.allow-methods	允许跨域的请求方式，默认OPTIONS,HEAD,GET,POST,PUT,DELETE
    http.cors.allow-headers	跨域允许设置的头信息，默认为X-Requested-With,Content-Type,Content-Length
    http.cors.allow-credentials	是否返回设置的跨域Access-Control-Allow-Credentials头，如果设置为true,那么会返回给客户端。
    ```
