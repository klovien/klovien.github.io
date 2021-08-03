---
layout:     post
title:      【JavaScript】promise对象
subtitle:   
date:       2021-08-02
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

### 封装一个promise格式的定时器

```javascript
function timeOut(delay) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            resolve();
        }, delay);
    })
}
```
### 调用
then写法
```javascript
timeOut(5000).then(() => {
    console.log('5秒结束');
})
```
await写法（必须在async函数中）
```javascript
//async函数里，顺序执行，将 异步=>同步，符合同步编写习惯
async function fiveSecond() { 
    await timeOut(5000);
    console.log("5秒结束");
}
```

