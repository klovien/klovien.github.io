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

# 注释 

<table><thead><tr><th align="center">注释名</th><th align="center">语法</th><th align="center">含义</th><th align="center">示例</th></tr></thead><tbody><tr><td align="center">@param</td><td align="center"><code>@param 参数名 {参数类型} 描述信息</code></td><td align="center">描述参数的信息</td><td align="center"><code>@param name {String} 传入名称</code></td></tr><tr><td align="center">@return</td><td align="center"><code>@return {返回类型} 描述信息</code></td><td align="center">描述返回值的信息</td><td align="center"><code>@return {Boolean} true:可执行;false:不可执行</code></td></tr><tr><td align="center">@author</td><td align="center"><code>@author 作者信息 [附属信息：如邮箱、日期]</code></td><td align="center">描述此函数作者的信息</td><td align="center"><code>@author 张三 2015/07/21</code></td></tr><tr><td align="center">@version</td><td align="center"><code>@version XX.XX.XX</code></td><td align="center">描述此函数的版本号</td><td align="center"><code>@version 1.0.3</code></td></tr><tr><td align="center">@example</td><td align="center"><code>@example 示例代码</code></td><td align="center">演示函数的使用</td><td align="center"><code>@example setTitle(‘测试’)</code></td></tr></tbody></table>

```javascript
/**
 * @method
 * @param {string} data 目标对象
 * @returns {string} 运营商名称
 * @desc 根据参数返回一个字符串
 */
function matchedNumber(data){
    return data + '返回一个字符串'
}
```



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

## 字符串方法

以截掉字符串最后一位为例

1.slice方法

```javascript
/**
 * slice(start,end)
 * start 要截取的字符串的起始下标 如果为负数从后面开始算起 -1指的是字符串的最后一位
 * end 要截取的字符串的结尾下标 如果为负数从后面开始算起 -1指的是字符串的最后一位
 * start 和 end 都是下标
 */

let str ="122889,"
str=str.slice(0,str.length-1)
console.log(str)//122889
```

2.substr方法

```javascript
/**
 * substr(start,length)
 * start 要截取的字符串的起始下标 如果为负数从后面开始算起 -1指的是字符串的最后一位
 *length 要截取的字符串的长度
 */

let str ="122889,"
str=str.substr(0,str.length-1)
console.log(str)
```

3.substring方法

```javascript
/**
 * substring(from,to)
 * from 要截取的字符串的起始下标 
 * to 截取到的字符串的下标 不包括这一项
 */

let str ="122889,"
str=str.substring(0,str.length-1)
console.log(str)
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

### 两种栈类的写法

```javascript
/**
 * @class 栈类
 * @description 继承自数组
 */
class stack extends Array {
    constructor() {
        super();
    }
    top() {
        return this[this.length - 1]
    }
    isEmpty() {
        return this.length === 0
    }
    size() {
        return this.length
    }
    clear() {
        this.length = 0
    }
    print() {
        console.log(this.toString())
    }
}
/**
 * 只实现了栈，无数组原生的方法
 */
function stack() {
    let items = []
    this.push = function (element) {
      items.push(element)
    }
    this.pop = function () {
      return items.pop()
    }
    this.peek = function () {
      return items[items.length - 1]
    }
    this.isEmpty = function () {
      return items.length === 0
    }
    this.size = function () {
      return items.length
    }
    this.clear = function () {
      items = []
    }
    this.print = function () {
      console.log(items.toString())
    }
}
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



```javascript
1、export default 向外暴露的成员，可以使用任意变量来接收

2、在一个模块中，export default 只允许向外暴露一次

3、在一个模块中，可以同时使用export default 和export 向外暴露成员

4、使用export向外暴露的成员，只能使用{  }的形式来接收，这种形式，叫做【按需导出】

5、export可以向外暴露多个成员，同时，如果某些成员，在import导入时，不需要，可以不在{ }中定义

6、使用export导出的成员，必须严格按照导出时候的名称，来使用{ }按需接收

7、使用export导出的成员，如果想换个变量名称接收，可以使用as来起别名
// test.js
var info = {
    name: 'zs',
    age: 20
}
export default info
export var title = '小星星'
export var content = '哈哈哈'
import {title, content as content1} from './test.js
```



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



# 判断类型是否为数字

我们先创建一些变量：

```
let intVar = 2;

let floatVar = 10.5;

let stringVar = '4';

let nanVar = NaN;

let infinityVar = Infinity;

let nullVar = null;

let undefinedVar = undefined;
```

**使用 Number.isFinite() 函数名**

`Number.isFinite()`函数检查变量是否为数字，还检查其是否为**有限值**。 因此，对于`NaN`，`Infinity`或`-Infinity`的数字，它返回`false`。

我们用上面定义的变量来检验一下:

```
> Number.isFinite(intVar);

true

> Number.isFinite(floatVar);

true

> Number.isFinite(stringVar);

false

> Number.isFinite(nanVar);

false

> Number.isFinite(infinityVar);

false

> Number.isFinite(nullVar);

false

> Number.isFinite(undefined);

false
```

这正是我们想要的。特殊的非有限数以及非数字类型的任何变量都会被忽略。所以，如果你想检查一个变量是否是一个数字，最好的方法是使用`Number.isFinite()`函数。

**使用 Number.isNaN() 方法**

标准`Number`对象有一个`isNaN()`方法。它接受一个参数，并确定其值是否为`NaN`。因为我们想检查一个变量是否是一个数字，所以我们将在检查中使用非操作符`!`。

```
> !Number.isNaN(intVar);

true

> !Number.isNaN(floatVar);

true

> !Number.isNaN(stringVar);

true # Wrong

> !Number.isNaN(nanVar);

false

> !Number.isNaN(infinityVar);

true # Wrong

> !Number.isNaN(nullVar);

true # Wrong

> !Number.isNaN(undefinedVar);

true # Wrong
```

这个方法是相当宽松的，因为它接受根本不是数字的值。这种方法最适合于当你知道你有一个数字并且要检查它是否是一个`NaN`值时，而不是一般的数字检查。

**使用 typeof() 方法**

`typeof()`函数是一个全局函数，它接受变量或值作为参数，并返回其类型的字符串表示。JavaScript 总共有`9`种类型

- undefined
- boolean
- number
- string
- bigint
- symbol
- object
- null (typeof() 显示的是 `object`)
- function (一种特殊类型的 object)

为了验证变量是否为数字，我们只需要检查`typeof()`返回的值是否为`"number"。 让我们尝试一下测试变量：

```
> typeof(intVar) == 'number';

true

> typeof(floatVar) == 'number';

true

> typeof(stringVar) == 'number';

false

> typeof(nanVar) == 'number';

true # Wrong

> typeof(infinityVar) == 'number';

true # Wrong

> typeof(nullVar) == 'number';

false

> typeof(undefined) == 'number';

false
```

`typeof()`函数的性能比`Number.isNaN()`要好得多。它正确地确定了字符串变量`null`和`undefined`不是数字。但是，对于`NaN`和`Infinity`，它返回`true`。

尽管从技术上来说这是正确的结果，但`NaN`和`Infinity`是特殊的数字值，对于大多数使用情况，我们宁愿忽略它们。

# ?. 与 ??

let arr = res && res.data && res.data.list

是不是非常不美观，今天介绍的新语法就是为了解决这种问题的

可选链操作符?.

来，用新语法再写一次

`let arr = res?.data?.list`

是不是很简洁了。

还有，要是想设置默认值怎么办

以前我们是这么写的

let arr = res && res.data || []

现在可以这样

 

`let arr = res?.res?.data ?? []`

这个??的意思是当左边的值为null或undefined的时候 就取??右边的值 。
