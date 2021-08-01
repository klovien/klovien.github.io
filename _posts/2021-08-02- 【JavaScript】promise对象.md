---
layout:     post
title:      【JavaScript】promise对象
subtitle:   
date:       2021-07-16
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

封装一个promise格式的定时器

```javascript
function timeOut(delay) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            resolve();
        }, delay);
    })
}

```

```javascript
timeOut(5000).then(() => {
    console.log('timeOut');
})
```

