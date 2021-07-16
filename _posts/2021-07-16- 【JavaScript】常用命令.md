---
layout:     post
title:      【JavaScript】常用命令
subtitle:   记录一下常用命令
date:       2021-07-16
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

## 二分搜索

返回第一个小于 等于 该数的下标 
**自己写的 **
( 相当于c++中的 lower_bound(begin,end,target) ;

```javascript
function binary_search(nums,target,low,high){
     if(low>=high)
      return low;
     let mid = low + parseInt( (high - low)/2 );
     if(nums[mid]>=target){
       return binary_search(nums,target,low,mid);  
     } 
     else if(nums[mid]<target){
        return binary_search(nums,target,mid+1,high);
     }
 }
```

## Math对象

一些数学中的特殊值

```javascript
Math.E			//e
Math.LN10		//ln10
Math.LN2		//ln2
Math.LOG2E		//log2e
Math.LOG10E		//lg e
Math.PI			//π
Math.SQRT2		//√2
Math.SQRT1_2	//√1/2
```

舍入方法

```JavaScript
Math.ceil(num)		//向上取整
Math.floor(num)		//向下取整
Math.round(num)		//四舍五入
Math.fround(num)	//返回数值最接近的单精度（32位）浮点值
```

随机数

```javascript
Math.random()  	//返回0~1范围随机数  
//let num = Math.random()*100 + 10; //10~109的数
```

其他方法

```javascript
Math.abs(num)  	//绝对值
Math.exp(num)	//e的num次幂
Math.expm1(num)	//e的num次幂 - 1
Math.log(num)	//ln num 
Math.pow(x,power)//x的power次幂
Math.sqrt(num)	//平方根
Math.cbrt(num)	//立方根
Math.acos
	acosh 	asin     asinh   atan 	atanh    atan2(y,x)  cos sin tan
//反双曲余弦 反双曲正弦 反双曲正弦 反正切	反双曲正切  y/x的反正切
```

