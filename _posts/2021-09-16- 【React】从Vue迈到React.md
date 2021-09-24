---
layout:     post
title:      【React】从Vue迈到React
subtitle:   
date:       2021-09-16
author:     YeanSe
header-img: 
catalog: true
tags:
    - React
---

# React

单向数据流：`万物皆 Props`，主要通过 `onChange/setState()`的形式该更新数据，需要所以在 react 中需要关注受控组件的写法

```js
// 会报错，props的值不可修改
<input value={this.props.value}/>

// 在onChange调用setState修改数据,需要调用setState修改绑定数据
<input value={this.state.value} onChange={this.onChange}/>
复制代码
```

## 受控组件

![image.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/af615ffb5d2e4d05a0377d6cd53b6b7a~tplv-k3u1fbpfcp-watermark.image)



## 事件冒泡和事件捕获

### 事件流

事件流描述的是从页面中接受事件的顺序。但有意思的是，IE和Netscape开发团队居然提出了差不多完全相反的事件流的概念。IE的事件流是事件冒泡流，而Netscape Communicator的事件流是事件捕获流。



### 事件冒泡（event bubbling）

即事件开始时由最具体的元素接收，然后逐级向上传播到较为不具体的节点。

**所有现代浏览器都支持事件冒泡**，但在具体实现上还是有一些差别。IE5.5及更早版本中的事件冒泡会跳过`<html>`元素（从`<body>`直接跳到`document`）。IE9、Firefox、Chrome和Safari则将事件一致冒泡到window对象。



### 事件捕获（event capturing）

事件捕获的思想是不太具体的节点应该更早接收到事件，而最具体的节点应该最后接收到事件。

## 简而言之就是，捕获是父级元素先触发，而冒泡是子级元素先触发

**react中有默认的冒泡事件，而且有捕获事件，即结尾加了个capture**

***onDragEnter vs onDragEnterCapture，onDragLeave vs onDragLeaveCapture***

