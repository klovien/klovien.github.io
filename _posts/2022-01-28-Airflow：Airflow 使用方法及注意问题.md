---
layout:     post
title:      Airflow：Airflow 使用方法及注意问题
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Airflow
---

> 关于 Airflow 的介绍，请参考 <a href="https://dex0423.github.io/2022/01/27/ETL-ETL%E7%9A%84%E5%9F%BA%E6%9C%AC%E6%A6%82%E5%BF%B5/">《ETL：ETL的基本概念》</a> 中 ***5.3. Airflow*** 一节的内容。

# 1. 安装配置

##### 1.1. 安装步骤

```
sudo apt update
sudo apt install python3-pip
pip3 install --upgrade pip
```

##### 1.2. 配置文件安装路径及环境变量

```
export AIRFLOW_HOME=~/airflow
pip3 install apache-airflow
export AIRFLOW_HOME=/home/work/airflow
echo 'export PATH=/usr/local/bin:$PATH' >>~/.bashrc
source ~/.bashrc
```

##### 1.3. 初始化 airflow

```
airflow db init
```

##### 1.4. 配置 webserver

```
vim /home/work/airflow/airflow.cfg
```
- 修改 webserver 配置
```
[webserver]
security = Flask AppBuilder
secure_mode = True
rbac=True
```

##### 1.5. 重置 airflow

```
airflow db reset
```

##### 1.6. 创建账号密码

```
airflow users create --lastname user --firstname admin --username name --email xxx@xxx.com --role Admin --password password
```

# 2. 启动访问

##### 2.1. 启动服务

```
airflow webserver --port 8080 --hostname 0.0.0.0        # 注意：ubuntu如果有开启防火墙，则需要允许端口8080通过
```
##### 2.2. web 访问
```
http://xxx.xxx.xxx.xxx:8080/home
账号：xxxx
密码：xxxx
```

# 3. DAGS

##### 3.1. DAG 概念简介

- dags 是用 python 编写的任务调度文件，用于给 airflow 要执行的 task 做任务计划。

##### 3.2. 文件位置

- dags 文件保存在该目录下
    ```
    /home/work/airflow/dags
    ```

##### 3.3. DAGS 样例文件

- sample.py

    ```aidl
    # coding: utf-8
    
    from airflow import DAG
    # from airflow.operators.python_operator import PythonOperator
    from airflow.operators.bash_operator import BashOperator
    from datetime import datetime, timedelta
    
    
    default_args = {
        'owner': 'admin',
        'depends_on_past': False,
        'start_date': datetime(2022, 3, 24),
        # 'email': ['xxx@xxx.com'],
        # 'email_on_failure': True,
        # 'email_on_retry': True,
        'retries': 3,
        'retry_delay': timedelta(seconds=5)
    }
    
    
    dag = DAG(
        dag_id='dws_delivery_reach_result_test',
        default_args=default_args,
        # schedule_interval="00, *, *, *, *"
        schedule_interval=timedelta(minutes=1)
    )
    
  
    first_task = BashOperator(
        task_id='dws_delivery_reach_result_test_1',
        depends_on_past=False,
        bash_command='python /home/work/airflow/tasks/xxxxx.py',
        dag=dag
    )
  
    second_task = BashOperator(
        task_id='dws_delivery_reach_result_test_2',
        depends_on_past=False,
        bash_command='python /home/work/airflow/tasks/xxxxx.py',
        dag=dag
    )
    
    first_task >> second_task
  
    ```

##### 3.4. 执行 dags 文件

```aidl
python sample.py
```
如果不报错就是执行成功。

##### #、3.5. 检查 dags 是否生效

```aidl
airflow dags list
```

如果 `paused` 值为 `None`，则表示 scheduler 中并未引入 dag。

```aidl
ag_id                           | filepath | owner | paused
=================================+==========+=======+=======
dws_delivery_reach_result_test_1 | test.py  | admin | False
```
如果 `paused` 值为 `True`，则表示 dags 生效，在webUI中可以查看到该 dag。

```aidl
dag_id                           | filepath | owner | paused
=================================+==========+=======+=======
dws_delivery_reach_result_test_1 | test.py  | admin | True
```

# 4. TASKS

##### 4.1. 概念简介

- task 是用来做数据清洗同步的具体任务，内容是用 python 编写的 SQL 相关语句，数据清洗工作在此处完成；

##### 4.2. 文件位置

- task 文件存放在该目录下：

  `/home/work/airflow/tasks/`

##### 4.3. dags 调度任务

>注意：
>- 使用 BashOperator，以命令行的形式执行 `python /home/work/airflow/tasks/xxxxx.py`；

##### 4.4. 频率设置

以下两种方式二选一：

- 定时任务：
    - `schedule_interval="00, *, *, *, *"`

- 时间间隔
    - `schedule_interval=timedelta(minutes=1)`


# 5. 注意事项

##### 5.1. dags 文件禁止汉字

- dag 文件中不能出现汉字，否则 dags 不会生效。

##### 5.2. 注意 cfg 文件中默认时间

`default_timezone = Asia/Shanghai`

##### 5.3. 推荐使用 BashOperator，不要使用 PythonOperator。

- 尽量不要使用 PythonOperator，不然 task 脚本会一直出现 import 导入路径报错。


