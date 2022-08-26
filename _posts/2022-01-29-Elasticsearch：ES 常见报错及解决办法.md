---
layout:     post
title:      Elasticsearch：ES 常见报错及解决办法
subtitle:   
date:       2022-01-29
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Elasticsearch
---


#### 处理 AttributeError: 'Response' object has no attribute 'my_suggest' 异常

- 环境
  - windows 10；
  - elasticsearch 7.8.0；
  - Django

- 问题
  - 使用 elasticsearch + Django 搭建搜索引擎，在获取 suggest 时遇到 **AttributeError: 'Response' object has no attribute 'my_suggest'** 报错，
  - 问题代码如下：
  ```
  from SEARCH.models import ArticleType  # 注意：ArticleType 是在另一个文件单独定义的！
  from django.http import HttpResponse
  from elasticsearch import Elasticsearch
  from datetime import datetime
  
  client = Elasticsearch(hosts=["127.0.0.1"])
  
  s = ArticleType.search()
  s = s.suggest(
      'my_suggest',
      "xx",
      completion={
          "field": "suggest",
          "fuzzy": {
              "fuzziness": 2
          },
          "size": 10
      }
  )
  suggestions = s.execute()
  ```
- 报错如下：
  >KeyError: 'my_suggest'
  >
  >During handling of the above exception, another exception occurred:
  >
  >Traceback (most recent call last):
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\django\core\handlers\exception.py", line 34, in inner
  response = get_response(request)
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\django\core\handlers\base.py", line 115, in _get_response
  response = self.process_exception_by_middleware(e, request)
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\django\core\handlers\base.py", line 113, in _get_response
  response = wrapped_callback(request, *callback_args, **callback_kwargs)
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\django\views\generic\base.py", line 71, in view
  return self.dispatch(request, *args, **kwargs)
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\django\views\generic\base.py", line 97, in dispatch
  return handler(request, *args, **kwargs)
  File "C:\Users\Administrator\Desktop\E_engine\E_search\SEARCH\views.py", line 36, in get
  for match in suggestions.my_suggest[0].options:
  File "C:\Users\Administrator\AppData\Local\Programs\Python\Python37\lib\site-packages\elasticsearch_dsl\utils.py", line 130, in __getattr__
  '{!r} object has no attribute {!r}'.format(self.__class__.__name__, attr_name))
  AttributeError: 'Response' object has no attribute 'my_suggest'


- 原因
  - elasticsearch 版本更新导致用法变更，问题代码是旧的写法，需要将 **s.execute_suggest()**  变更为  **s.execute().to_dict()**；

- 解决：
  - 更新代码
  
    ```
    from django.http import HttpResponse
    from elasticsearch import Elasticsearch
    from datetime import datetime
  
    client = Elasticsearch(hosts=["127.0.0.1"])
  
    s = ArticleType.search()
    s = s.suggest(
        'my_suggest',
        "九州",
        completion={
            "field": "suggest",
            "fuzzy": {
                "fuzziness": 2
            },
            "size": 10
        }
    )
    # 调用 execute_suggest 方法
    # suggestions = s.execute()
    suggestions = s.execute().to_dict()    # 此处将 s.execute_suggest()  变更为  s.execute().to_dict()
    print("#" * 50)
    print(suggestions)
    print("#" * 50)
    ```


#### 处理 analyzer [ik_max_word] not found for field [name] 异常

- 环境
  - windows 10；
  - elasticsearch 7.8.0；

- 问题
  - elasticsearch 创建索引时遇到 analyzer [ik_max_word] not found for field [name] 报错；
  - 报错：
    >org.elasticsearch.index.mapper.MapperParsingException: analyzer [ik_max_word] not found for field [name]

- 原因
  - IK分词器插件 未安装；

- 解决
  - 进入 elasticsearch 安装目录下的 bin\ 目录，在此目录打开命令行窗口，输入下面代码安装IK分词器插件；
  ```
  elasticsearch-plugin install https://github.com/medcl/elasticsearch-analysis-ik/releases/download/vX.X.X/elasticsearch-analysis-ik-X.X.X.zip
  ```
  - 安装成功，如下图：

    ![]({{site.baseurl}}/img-post/es-13.png)

  - 重启 elasticsearch，重新创建索引，不在报错、创建成功；


#### elasticsearch-head 连接不上 Elasticsearch，localhost:9100 显示集群健康值：未连接

- 环境
  - windows 10；
  - elasticsearch 7.8.0；
  - elasticsearch-head；

- 问题
  - 启动 elasticsearch 和 elasticsearch-head 以后，[http://localhost:9100/](http://localhost:9100/)  页面显示 **集群健康值：未连接**，点击 连接 按钮以后没有任何反应；

  ![]({{site.baseurl}}/img-post/es-11.png)

- 解决
  - 原因：config/elasticsearch.yml 文件的配置问题；
  - 进入 elasticsearch/config 文件夹，打开 elasticsearch.yml 文件，在文件尾部添加如下代码；
  ```
  # 开启跨域访问支持，默认为false
  http.cors.enabled: true
  # 跨域访问允许的域名地址
  http.cors.allow-origin: "*"
  # 通过为 cluster.initial_master_nodes 参数设置符合主节点条件的节点的 IP 地址来引导启动集群
  cluster.initial_master_nodes: ["node-1"]
  ```
  - 保存文件后重启 elasticsearch 和 elasticsearch-head；
  - 问题解决，如下图：

  ![]({{site.baseurl}}/img-post/es-12.png)