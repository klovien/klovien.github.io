---
layout:     post
title:      【JavaScript】常用方法
subtitle:   记录一下常用方法
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
( 相当于c++中的反版 lower_bound(begin,end,target) ;

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

**一些数学中的特殊值**

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

**舍入方法**

```JavaScript
Math.ceil(num)		//向上取整
Math.floor(num)		//向下取整
Math.round(num)		//四舍五入
Math.fround(num)	//返回数值最接近的单精度（32位）浮点值
```

**随机数**

```javascript
Math.random()  	//返回0~1范围随机数  
//let num = Math.random()*100 + 10; //10~109的数
```

**其他方法**

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

## 数组方法

#### 1.构建

```javascript
let arr =[];
let arr = new Array(20);			//有20个空元素
let arr = new Array("ele1","ele2"); //2个元素
let arr = Array.from(map);			//类数组 -> 数组 （arguments伪数组，字符串，map，set等）
let arr = Array.of(1,2,3,4);		//一组参数 ->数组
```

#### 2.检测

```javascript
Array.isArray(value)     //确定一个值是不是数组
value instanceof Array   //只适用于一个页面
```

#### 3.迭代

| 迭代器      |        |
| :---------- | ------ |
| a.keys()    | 键     |
| a.values()  | 值     |
| a.entries() | 键值对 |
```javascript
a.forEach( (item,index,array) =>{ } );


//params：前一次函数的返回值，当前元素，和当前元素索引 和 array的本体
Array.reduce(function(prev,curr,index,array){...})
Array.reduceRight(function(prev,curr,index,array){...})//同理，从末尾往前迭代
```

![]({{site.baseurl}}/img-post/js-20210718.png)

#### 4.复制和填充

```javascript
a.fill(5);   	//用5填充整个数组
a.fill(6,3);	//用6填充 下标 3<=index
a.fill(6,1,5); 	//用6填充 下标 1<=index<5
```

```javascript
a.copyWithin(5);	//从0<=index开始浅拷贝，插入到 5 	-> 0 1 2 3 4 0 1 2 3 4 5
a.copyWithin(5,1);	//从1<=index开始浅拷贝，插入到 5	-> 0 1 2 3 4 1 2 3 4 5 6
a.copyWithin(5,1,2);//从1<=index<2开始浅拷贝，插入到5	-> 0 1 2 3 4 1 5 6 7 8 9
```

#### 5.转换

```javascript
a.toString()
a.valueOf()
a.toLocaleString()
//常用：
let a = [1,2,3,4,5,6]
let str = ''.concat(...a);
//或者用a.toString().replace(/,/g,'');
//1,2,3,4,5,6 => 123456

let string = "this is a string";
//string => array
arr = [...string]
//array => string
let str = ''.concat(...arr);
```

![]({{site.baseurl}}/img-post/JSstring-20210724.png)

#### 6.栈

push返回最新长度 pop返回删除元素

```javascript
a.push("a");		//["a"] 
a.push("a","b");	//["a","a","b"] 可以同时push两个 
console.log(a.pop()); //b 删掉了b
```

#### 7.队列

```javascript
a.unshift("a");		//["a"] 
a.unshift("a","b");	//["a","b","a"] 可以 前插 两个,还是按原ab顺序 
console.log(a.shift()); //a 删掉了a
```

#### 8.排序

```javascript
a.sort()
a.sort(compare)
a.reverse()
```

#### 9.操作

##### concat 

衔接无限个数组、元素，返回一个新构建的数组

```javascript
let newArray = a.concat("a",["a","b"])  //创建一个 ["a","a","b"]
```

##### slice   

 slice(开始,结束)   返回 开始<=index<结束 的新数组

```javascript
let newArray = a.slice(0)  //创建一个 0到末尾
let newArray = a.slice(0,1)  //创建一个 [0] 
```

##### splice 

最强大的数组操作方法 一次实现 删除、插入、替换 三个功能的语法糖

| 功能 | 参数                           | 语法糖                          | 用途                        |
| ---- | ------------------------------ | ------------------------------- | --------------------------- |
| 删除 | 2       开始位置 删除数量      | a.splice(begin,num)             | 删除begin<=index的num个元素 |
| 插入 | >=3  开始位置 0(删除数量) 元素 | a.splice(begin,0,ele...)        | 在begin处 插入 元素         |
| 替换 | >=3  开始位置 num num个元素    | a.splice(begin,num,num个ele...) | 又删又插 就是替换           |

#### 10.搜索方法

| 方法                                           | 返回    | 功能                   |
| ---------------------------------------------- | ------- | ---------------------- |
| a.includes(ele,begin)                          | bool    | 是否含有               |
| a.indexOf(ele,begin)                           | 下标    | 从起始查找第一个       |
| a.lastIndexOf(ele,begin)                       | 下标    | 从末尾查找第一个       |
| a.find( (ele,index,array) => ele.age<18 )      | 某个ele | 第一个匹配的元素       |
| a.findIndex( (ele,index,array) => ele.age<18 ) | 某个ele | 第一个匹配的元素的索引 |

