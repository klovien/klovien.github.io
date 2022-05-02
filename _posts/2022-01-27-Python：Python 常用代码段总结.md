---
layout:     post
title:      Python：Python 常用代码段总结
subtitle:   
date:       2022-10-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - Python
---


# 使用 try except import 第三方工具包

##### 需求
- 在 import 第三方工具包时，使用 try ... except ... 进行导入，以提高代码健壮性。

##### 实现

- 此处以导入 pandas 为例进行演示
  ```
  import os  # 注意：此包必须提前直接导入
  try:
      import pandas as pd
  except:
      os.system('pip install pandas')  # 注意 pip 和 pip3 在使用时的区别
      import pandas as pd
  
  frame = pd.read_csv('file.csv', engine='python')
  ...
  ```
  
# 统计程序执行时间

##### 需求

- 在 python 程序中，记录程序开始结束时间，用于统计程序运行时长。

##### 实现

- 方法一：普通方法
  ```
  import datetime
  
  def main():
      ...    # 程序运行函数
  
  if __name__ == '__main__':
      starttime = datetime.datetime.now()    # 记录开始时间
      
      main()    # 运行主程序
      
      endtime = datetime.datetime.now()    # 记录结束时间
      print (endtime - starttime).seconds    # 以秒为单位，计算运行时长
  ```
- 方法二：装饰器
  ```
  import time
  from functools import wraps
    
  def fn_timer(function):
    @wraps(function)
    def function_timer(*args, **kwargs):
      t0 = time.time()
      result = function(*args, **kwargs)
      t1 = time.time()
      print ("Total time running %s: %s seconds" %
          (function.func_name, str(t1-t0))
          )
      return result
    return function_timer
  
  @fn_timer
  def main():
      ...    # 程序运行函数
  
  if __name__ == '__main__':
      main()    # 运行主程序
  ```

# 以追加方式写入数据到 csv 文件中

##### 需求

- python 打开文件，并以追加方式写入数据到 csv 文件中。

##### 解决
- 代码
  
  ```
  import csv
  
  def write_csv(path, data_row):
      with open(path,'a+') as f:
          csv_write = csv.writer(f)
          csv_write.writerow(data_row)
  
  path  = "file_name.csv"
  data_row = ["1","2"]
  write_csv(path, data_row)
  
  ```
##### 补充

>w：以写方式打开， 
a：以追加模式打开 (从 EOF 开始, 必要时创建新文件) 
r+：以读写模式打开 
w+：以读写模式打开 (参见 w ) 
a+：以读写模式打开 (参见 a ) 
rb：以二进制读模式打开 
wb：以二进制写模式打开 (参见 w ) 
ab：以二进制追加模式打开 (参见 a ) 
rb+：以二进制读写模式打开 (参见 r+ ) 
wb+：以二进制读写模式打开 (参见 w+ ) 
ab+：以二进制读写模式打开 (参见 a+ )


# 新建 csv 文件写入第一行标题，并跳过第一行追加写入数据

##### 需求

- 如题，本文的目标是新建 csv 文件，在第一行写入标题，并追加写入数据。这里需要注意，标题写入第一行以后，后续就会自动跳过。

##### 解决

- 代码
  ``` 
  import csv
  
  
  def write_to_csv(file_name, item):
    '''
    :params file_name: 保存的文件名
    :params item: 要保存的额数据         # 此处保存的数据是 dict 字典格式的
    '''
    with open("{}.csv".format(file_name), "a+", newline='') as csv_writer:
          writer = csv.writer(csv_writer)
          #以读的方式打开csv 用csv.reader方式判断是否存在标题。
          with open("test.csv", "r", newline="") as csv_reader:
              reader = csv.reader(csv_reader)
              if not [row for row in reader]:
                  writer.writerow(item.keys())     # keys 作为第一行标题
                  writer.writerows(item.values())    # 追加写入 values 值
              else:
                  writer.writerows(item.values())    # 追加写入 values 值
  
  
  def main():
      file_name = 'csv_file_name'
      item = {
                  'key_1': 'value_1',
                  'key_2': 'value_2'
              }
      write_to_csv(file_name, item)
  
  if __name__ == '__main__':
      main() 
  ```  
  
# 按 key 首字母位置对字典键值对进行重新排序

##### 需求

- 现有一个字典，如下：
  ```
  dict_1 = {
            'c_key': 'c_value',
            'a_key': 'a_value',
            'd_key': 'd_value',
            'b_key': 'b_value',
            }
  ```
- 需要将其按照 ‘a_key’, 'b_key', 'c_key', 'd_key' 的顺序，对 dict_1 中的键值对进行重新排序。
##### 实现
- 代码
  ```
  dict_2 = dict(sorted(dict_1.items(), key=lambda i:i[0]))
  print(dict_2)
  ```
  得到结果如下：
  ```
  {'a_key': 'a_value', 'b_key': 'b_value', 'c_key': 'c_value', 'd_key': 'd_value'}
  ```