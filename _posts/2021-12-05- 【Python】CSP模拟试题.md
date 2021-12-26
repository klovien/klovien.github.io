---
layout:     post
title:      【Python】CSP模拟试题
subtitle:   
date:       2021-12-05
author:     YeanSe
header-img: 
catalog: true
tags:
    - Python3

---

# 202109-1- 数组推导

```python
n = int(input())
B = list(map(int, input().split()))
sumMAX = B[0]
sumMIN = B[0]

max = B[0]
for i in range(1, n):
    if B[i] <= max:
        sumMAX += max
        sumMIN += 0
    else:
        max = B[i]
        sumMAX += B[i]
        sumMIN += B[i]
print(sumMAX)
print(sumMIN)
```

```python
redTime, yellowTime, greenTime = input().split()
redTime, yellowTime, greenTime = int(redTime), int(yellowTime), int(greenTime)
num = int(input())
res = 0


def judgeLight(action, second):
    if action == 1:
        return second
    elif action == 2:
        return redTime  + second
    elif action == 3:
        return 0
    else:
        return second


for i in range(0, num):
    action, second = input().split()
    action, second = int(action), int(second)
    usedTime = judgeLight(action, second)
    # print(usedTime)
    res += usedTime
print(res)

```

```python
n = int(input())
list1 = [0] + [int(x) for x in input().split()] + [0]
xh = sorted(set(list1))
print("数组去重之后", xh)
dic = {}
for x in range(1, len(list1) - 1):
    if list1[x] in dic:
        dic[list1[x]].append(x)
    else:
        dic[list1[x]] = [x]
print("值:索引", dic)
pri = 0
num = 0
for x in range(len(list1) - 1):
    if list1[x] == 0 and list1[x + 1] > 0:
        num += 1
print("当前非零段个数为", num)

for x in xh:
    if x == 0:
        continue
    for y in dic[x]:
        list1[y] = 0
        if list1[y + 1] > 0 and list1[y - 1] > 0:
            num += 1

        elif list1[y + 1] == 0 and list1[y - 1] == 0:
            num -= 1
    pri = max(pri, num)
print(pri)

```

