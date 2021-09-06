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

```javascript
function binary_search(numbers,target,low,high,) {
        let left = low;
        let right = high;
        if (target > numbers[right] || target < numbers[left])
            return -1;
        while (left <= right) {
            let mid = left + parseInt((right - left) / 2);
            if (numbers[mid] == target) {
                return mid;
            } else if (numbers[mid] < target) {
                left = mid + 1;
            } else if (target < numbers[mid]) {
                right = mid - 1;
            }
        }
        return -1;
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
Math.random()  //返回0<=x<1范围的随机数  
//let ran = Math.random()*100 + 10; 	//10~109的数
//let ran = ~~Math.random()*100 + 10;	//10~109的整数
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

##### 5个迭代方法，参数均为	 function(item,index,array){}

**1、every（）∀ 任意**：对数组中的每一项运行给定函数，如果该函数对每一个项都返回true,则返回true
**2、some（）∃存在	：**对数组中的每一项运行给定函数，如果该函数对任一一项返回true,则返回true
**3、filter（）过滤器	：**对数组中的每一项运行给定函数，返回该函数会返回true的项组成的数组

**4、foreach（）		：**对数组中的每一项运行给定函数，没有返回值
**5、map（）映射转换：**对数组中的每一项运行给定函数，返回每次函数调用的结果组成的数组

##### 2个归并方法 reduce和reduceRight

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
let str = a.join('');			//123456
let str = ''.concat(...a);	//123456
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

# 本地存储

localStorage 是 HTML5 标准中新加入的技术，用于长久保存整个网站的数据，保存的数据没有过期时间，直到手动去删除；
localStorage和sessionStorage最大一般为5MB，仅在客户端（即浏览器）中保存，不参与和服务器的通信；

setItem(key,value) 设置数据
getItem(key) 获取数据
removeItem(key) 移除数据
clear() 清除所有值

sessionStorage仅在当前会话下有效，关闭页面或浏览器后被清除；

setItem(key,value) 设置数据

getItem(key) 获取数据

removeItem(key) 移除数据

clear() 清除所有值



# 字符串方法

## ASCII码 转 字符

### `String.fromCodePoint(num1[, ...[, numN]])`

```
String.fromCharCode(65) // A
String.fromCharCode(90) // Z
String.fromCharCode(97) // a
String.fromCharCode(122) // z
```

### `String.fromCharCode(num1[, ...[, numN]])`

```
String.fromCodePoint(65) // A
String.fromCodePoint(90) // Z
String.fromCodePoint(97) // a
String.fromCodePoint(122) // z
```

`fromCodePoint()`, `fromCharCode()`两个都是`String`的静态方法，所以直接使用，不需要实例化。

两者的主要区别是：
`fromCharCode()` 出现的早，可以处理常用的字符编码
`fromCodePoint()` ES2015出现的，可以处理高位编码。

## 字符 转 ASCII码

### `'A'.charCodeAt() // 65`

```
'A'.charCodeAt() // 65
'a'.charCodeAt() // 97
'Z'.charCodeAt() // 90
'z'.charCodeAt() // 122
```

# 模块化语法

## ES5 `module.export ={} `和 `require()`

```javascript
//工具人.js
module.exports = {//挺好，直接封装成对象，函数中可以调用 this
  	a,
    b
}
或者用 exports = {} //是一样的
//正常写.js
const test = require("./test.js")
```

## ES6 `export default`和`import 内容 from 路径`

```javascript
//工具人.js
//语法
export var a = 1;
或
var a = 1;
export default a;

export default {//80%以上的情况，默认暴露
  a:"封装成一个对象，写法是一样的", 
  方法1(){
    
  }
}

//正常写.js
import a from "./test.js"
```

利用es6规范，还想用node运行js文件，想的美。必须用babel将es6 -> es5 

```shell
npm init -y
npm install babel-preset-es2015
babel 文件/文件夹名 -d dist
```

或者在插件商店找个compile to es5 的插件 例如：compile hero

# 进制转换

#### 1.10进制转其他进制

```javascript
//number类型的toString方法
(100).toString(2)         // "1100100"
(100).toString(8)         // "144"
(100).toString(16)         // "64"
```



#### 2.其他进制转 十进制

`parseInt(数，该数的进制)`

##### 16进制转10进制

```javascript
parseInt(0x64)            //  100 
parseInt('0x64')          //  100
parseInt('0x64',16)       //  100    第二参数写了更好，以免迷糊
parseInt('ff')            //  NaN
parseInt('ff',16)         //  255   这里没有0x打头，必须要注明要转的基数
```

##### 8进制转10进制

```javascript
parseInt(0100);           //  64    先转成字符串，再以8进制解析
parseInt("0100",8);       //  64    正确写法，无歧义  !!!

parseInt("0100");         //  100   官方表示不确定：但是测试结果，谷歌浏览器当成10进制输入。所以不建议这样写
```



##### 2进制转10进制

```javascript
parseInt("11",2);        //返回 3 
```

##### **补充**

ES6支持二进制和八进制的字面量

　　1、在**数字**前面添加 **0o** 或者 **0O** 即可将其转换为八进制

　　2、在**数字**前面添加 **0b** 或者 **0B** 即可将其转换为二进制

```
let oValue = 0o10;
console.log(oValue);      //8

let bValue = 0b10;
console.log(bValue);      //2
```
