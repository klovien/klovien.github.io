---
layout:     post
title:      JS逆向：常见的无限debugger反调试策略以及应对方法
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - JS逆向
---


# 种类划分

### 按代码逻辑划分

##### 无限循环

- for 循环
- while 循环

> 注意：使用 while 循环一定会有次数上限，否则浏览器会卡死掉。

##### 无限递归

- 调用自身

#### 两个方法循环互调

- 顾名思义

##### 计时器

`setInterval` 这个是 JS 语言当中的 `定时器`，它有两个参数：第一个是要执行的代码，第二个执行时间。
下方是 sojson 混淆后的一段代码，其中使用了 `setInterval` 定时器进行了反 debugger 操作。

这个一段代码执行的话会一直断住，不管你怎么跳转到下一个断点，它都还是会一直断，其实这个就是一个无限debugger的代码。

### 按是否可混淆划分

##### 不可混淆

直接使用代码，

```
debugger;
```

##### 可混淆

```
eval("debugger;")
```

##### 可重度混淆

```
Function("debugger").call()/apply() 或赋值 bind()
```

```
xxx.constructor("debugger").call("action")
```

```
Fuction.constructor("debugger").call("action")
```

```
(function(){return !![];}["constructor"]("debugger")["call"]("action"))
```

### 按设置节点划分

- 无限 debugger 贯穿全局
  这种情况，多是由 `setInterval` 定时器实现的。针对这种情况，我们需要将定时器函数置空或者重写。
- 在加密业务逻辑之前设置
- 在加密业务逻辑之后设置

# 应对方法

### 针对静态文件

##### conditional breakpoint

在 JS 代码 debugger 行数位置，鼠标右键添加 conditional breakpoint，其中条件 condition 设为 false；

##### Fiddler AutoResponder 篡改 JS 代码
这种方式的核心思路，是替换 JS 文件中的 debugger 关键字，并保存为本地文件，在请求返回的时候、通过正则匹配等方式、拦截并替换返回的 JS 代码，已达到绕过 debugger 的目的。
关于这种方法，请参考我的另一篇文章：《[JS逆向：fiddler 篡改 js 破解企查查无限 debugger 问题](https://www.jianshu.com/p/9f72c4e0fd34)》，文中有详细的描述。

### 针对动态文件

##### 手动在浏览器中 Hook

##### 第一步、打 script 断点

这一步的目的，是为了让浏览器在刚运行时就被断下来，以方便进行 Hook

##### 第二步、Hook 无限 debugger 函数

- 手动置空 debugger 函数。
  打 `script` 断点，使网页在 debugger 之前下断，手动将debugger所在函数置空。这种方法简单粗暴，但是会影响原有的业务逻辑。
```
包含debugger函数 = function (){};
```

- 手动重写 debugger 函数。
  打 `script` 断点，然后在 debugger 函数被调用的地方下断，手动将debugger所在函数中的 debugger 逻辑删除，然后 **把 debugger 函数赋值给 调用函数**。
>注意：这一步赋值不能省，否则无法调用。

- Hook Funciton

    ```
    F_ = Function
    Function = function(a){
        if (a!=='debugger'){return F_(a)}
    }
    ```
- Hook Function 构造器函数
    ```
    Function.prototype.constructor_ = Function.prototype.constructor;
    Function.prototype.constructor = function(x){
        if (x!=='debugger'){return Function.prototype.constructor_(x)}
    }
    ```
如果觉得上面的代码太粗暴，也可使用下方代码。
    ```
    Function.prototype.__constructor_back = Function.prototype.constructor;
    Function.prototype.constructor = function() {
        if(arguments && typeof arguments[0]==='string'){
            //alert("new function: "+ arguments[0]);
            if("debugger" === arguments[0]){
                //arguments[0]="console.log(\"anti debugger\");";
                //arguments[0]=";";
                return
            }
        }
       return Function.prototype.__constructor_back.apply(this,arguments);
    }
    ```
- Hook eval 函数
    ```
    eval_ = eval;
    //下面这样写，是为了过瑞数的 eval.toString 检测
    eval = function(a){if(a=='debugger'){return ''}else{return eval_(a)}}    
    ```
- Hook conole.log，为了防止调试过程中，console.log 被重写，也可以在此时对 console.log 进行 hook，之后在 console.log 无法正常打印的位置，再将 console.log 进行复原；

Hook console.log：
    ```
    console.log_ = console.log
    ```
将 console.log 复原：
    ```
    console.log = console.log_
    ```
- Hook setInterval 函数
  - 业务代码和 setInterval 无关
    ```
    setInterval = function(){}
    ```
  - 业务代码和 setInterval 有关
    ```
    setInterval_back = setInterval
    setInterval = function(a,b){
        if(a.toString().indexOf('debugger') == -1){
          return null;
        }
        setInterval_back(a, b)
    }
    ```

##### Fiddler + 编程猫插件 + Hook

使用这种方法，就不需要再打 `script` 断点。
>注意：如果 `script` 断点无法在 debugger 函数之前断下来，那就只能用这种方式进行拦截 Hook，才能过掉 debugger。

使用编程猫插件用到的代码，和在浏览器中手动 Hook 基本一致。在 Hook `eval` 过瑞数 debugger 的时候，可以用到下面的代码。

```
//配合编程猫专用工具进行hook
(function() { 
'use strict';

//过 瑞数 debuger 
var eval_ = window.eval;
window.eval = function(x){
    eval_(x.replace("debugger;","  ; "));
};
//过 瑞数 debuger检测
window.eval.toString = eval_.toString;
})();
```