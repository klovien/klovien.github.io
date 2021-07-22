---
layout:     post
title:      【JavaScript】深浅拷贝
subtitle:   
date:       2021-07-16
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

```javascript
//浅拷贝
let obj = new Object(obj1);

function copy1(obj){
	let newObj = {};
	for(let i in obj){
		newObj[i]=obj[i];
	}
	return newObj;
}
//深拷贝
function copy2(obj) {
	let newObj = {};
	for (let i in obj) {
		if (obj[i] instanceof Object) {
			newObj[i] = copy2(obj[i]);
		} 
		else {
			newObj[i] = obj[i];
		}
	}
	return newObj;
}
//Json深拷贝
function copy3(obj){
	let tempStr = JSON.stringify(obj);
	let newObj = JSON.parse(tempStr);
	return newObj;
}
```

