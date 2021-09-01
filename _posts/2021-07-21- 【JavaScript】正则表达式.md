---
layout:     post
title:      【JavaScript】正则表达式
subtitle:   JavaScript
date:       2021-07-21
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

```javascript
pattern.exec(str)
```
学正则表达式的网址：https://codejiaonang.com/

<h2 id="section">多种匹配模式</h2>

<table>
  <thead>
    <tr>
      <th>实例</th>
      <th>描述</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>[Pp]ython</code></td>
      <td>匹配 “Python” 或 “python”。</td>
    </tr>
    <tr>
      <td><code>rub[ye]</code></td>
      <td>匹配 “ruby” 或 “rube”。</td>
    </tr>
    <tr>
      <td><code>[abcdef]</code></td>
      <td>匹配中括号内的任意一个字母。</td>
    </tr>
    <tr>
      <td><code>[0-9]</code></td>
      <td>匹配任何数字。类似于 [0123456789]。</td>
    </tr>
    <tr>
      <td><code>[a-z]</code></td>
      <td>匹配任何小写字母。</td>
    </tr>
    <tr>
      <td><code>[A-Z]</code></td>
      <td>匹配任何大写字母。</td>
    </tr>
    <tr>
      <td><code>[a-zA-Z0-9]</code></td>
      <td>匹配任何字母及数字。</td>
    </tr>
    <tr>
      <td><code>[^au]</code></td>
      <td>除了au字母以外的所有字符。</td>
    </tr>
    <tr>
      <td><code>[^0-9]</code></td>
      <td>匹配除了数字外的字符。</td>
    </tr>
  </tbody>
</table>

<table>
  <thead>
    <tr>
      <th>实例</th>
      <th>描述</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>.</code></td>
      <td>匹配除 “\n” 之外的任何单个字符。要匹配包括 ‘\n’ 在内的任何字符，请使用象 ‘[.\n]’ 的模式。</td>
    </tr>
    <tr>
      <td><code>?</code></td>
      <td>匹配一个字符零次或一次，另一个作用是非贪婪模式</td>
    </tr>
    <tr>
      <td><code>+</code></td>
      <td>匹配1次或多次</td>
    </tr>
    <tr>
      <td><code>*</code></td>
      <td>匹配0次或多次</td>
    </tr>
    <tr>
      <td><code>\b</code></td>
      <td>匹配一个长度为<code>0</code>的子串</td>
    </tr>
    <tr>
      <td><code>\d</code></td>
      <td>匹配一个数字字符。等价于 [0-9]。</td>
    </tr>
    <tr>
      <td><code>\D</code></td>
      <td>匹配一个非数字字符。等价于 [^0-9]。</td>
    </tr>
    <tr>
      <td><code>\s</code></td>
      <td>匹配任何空白字符，包括空格、制表符、换页符等等。等价于 [ \f\n\r\t\v]。</td>
    </tr>
    <tr>
      <td><code>\S</code></td>
      <td>匹配任何非空白字符。等价于 [^ \f\n\r\t\v]。</td>
    </tr>
    <tr>
      <td><code>\w</code></td>
      <td>匹配包括下划线的任何单词字符。等价于’[A-Za-z0-9_]’。</td>
    </tr>
    <tr>
      <td><code>\W</code></td>
      <td>匹配任何非单词字符。等价于 ‘[^A-Za-z0-9_]‘。</td>
    </tr>
    <tr>
      <td><code>\b</code></td>
      <td>匹配一个长度为<code>0</code>的子串</td>
    </tr>
  </tbody>
</table>
</div>



## 匹配方法

`语法：str.match(reg) 返回：找到的 子串或者子串组`

```javascript
const str = 'Hello JavaScript'
str.match()  // [""]
str.match(/JS/) // null
str.match(/JavaScript/g)  // ["JavaScript"]
str.match(/JavaScript/)  // [0: "JavaScript", groups: undefined, index: 6, input: "Hello JavaScript"]
str.match(/(?<J>Java)(?<S>Script)/)  // [0: "JavaScript", 1: "Java", 2: "Script", groups: { J: "Java", S: "Script" }, index: 6, input: "Hello JavaScript"]
```

`语法：str.matchAll(reg) `

```javascript
const regexp = /t(e)(st(\d?))/g;
const str = 'test1test2';
const array = [...str.matchAll(regexp)];

console.log(array[0]);
// expected output: Array ["test1", "e", "st1", "1"]

console.log(array[1]);
// expected output: Array ["test2", "e", "st2", "2"]


// 0: "hello"  匹配的字符串，如果有使用分组会在后面依次列出来
// groups: undefined  没有使用命名捕获组会返回undefined，否则会返回包含命名捕获组的对象
// index: 0  匹配的结果在当前字符串位置开始的索引
// input: "hello javascript hello css" 当前字符串
```



