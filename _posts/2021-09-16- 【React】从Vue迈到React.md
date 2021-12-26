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

## 创建项目

### Create React App 

[Create React App](https://github.com/facebookincubator/create-react-app) 是一个用于**学习 React** 的舒适环境，也是用 React 创建**新的[单页](https://react.docschina.org/docs/glossary.html#single-page-application)应用**的最佳方式。

它会配置你的开发环境，以便使你能够使用最新的 JavaScript 特性，提供良好的开发体验，并为生产环境优化你的应用程序。你需要在你的机器上安装 [Node >= 8.10 和 npm >= 5.6](https://nodejs.org/en/)。要创建项目，请执行：

```shell

1 npm 安装 react
npm i -g create-react-app
2初始携带ts的react项目
npx create-react-app my-app	或
npx create-react-app my-app --template typescript

cd my-app
npm start
```

> 注意
>
> 第一行的 `npx` 不是拼写错误 —— 它是 [npm 5.2+ 附带的 package 运行工具](https://medium.com/@maybekatz/introducing-npx-an-npm-package-runner-55f7d4bd282b)。

Create React App 不会处理后端逻辑或操纵数据库；它只是创建一个前端构建流水线（build pipeline），所以你可以使用它来配合任何你想使用的后端。它在内部使用 [Babel](https://babeljs.io/) 和 [webpack](https://webpack.js.org/)，但你无需了解它们的任何细节。

## 安装sass

```bash
yarn add sass
yarn add sass-loader
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

### 简而言之就是，捕获是父级元素先触发，而冒泡是子级元素先触发

**react中有默认的冒泡事件，而且有捕获事件，即结尾加了个capture**

***onDragEnter vs onDragEnterCapture，onDragLeave vs onDragLeaveCapture***

## react用 onChange  实现 v-modal双向绑定

如何获取输入框的值是react中必须面对的问题，目前比较好的方式与两种，一种是使用ref，而另一种是使用onChange事件，它通过事件对象的方式获取输入值。 
具体使用方式如下：

### onChange

```react
class Main extends Component{
    changeEvent(e){
        console.log(e.target.value);

    }
    render(){
        return (
            <div>
                <input type = 'text' onChange={(e)=>this.changeEvent(e)}/>
            </div>
        )
    }
}
```

## ref

### ref 回调函数

ref属性可以设置为一个回调函数，这也是官方强烈推荐的用法；这个函数执行的时机为：

- `组件被挂载后`，回调函数被立即执行，回调函数的参数为该组件的具体实例。
- `组件被卸载或者原有的ref属性本身发生变化时`，回调也会被立即执行，此时回调函数参数为`null`，以确保内存泄露。

```react
 RegisterStepTwo = React.createClass({
        getInitialState(){
          return {visible: true};
        },
      changeVisible(){
        this.setState({visible: !this.state.visible});
      },
      refCb(instance){
        console.log(instance);
      },
      render(){
        return(
          <div>
            <button type="button" onClick={this.changeVisible}>{this.state.visible ? '卸载' : '挂载'}ConfirmPass
            </button>
            {
              this.state.visible ?
                <ConfirmPass ref={this.refCb} onChange={this.handleChange}/>: null
             }
           </div>
         )
      }
    });

```

### ref 类属性 + React.createRef()

`ref`是`react`提供给我们的一个属性,通过它，我们可以访问 `DOM` 节点或者组件.

```react
// 父组件
import React from 'react'
import Son from './son'
import { Button } from 'antd'

class Father extends React.Component {
  son: any
  constructor(props) {
    super(props)
    this.son = React.createRef() // 在此处创建ref
  }
  clearSonInput = () => {
    const { current } = this.son // 注意，这里必须通过 this.son.current拿到子组件的实例
    current.clearInput()
  }
  render() {
    return (
      <div>
        <Son ref={this.son} />
        <Button type='primary' onClick={this.clearSonInput}>
          清空子组件的input
        </Button>
      </div>
    )
  }
}
export default Father

// 子组件
import React from 'react'
import { Button } from 'antd'

class Son extends React.Component {
  constructor(props) {
    super(props)
  }
  state = {
    info: 'son',
  }
  handleChange = (e) => {
    this.setState({
      info: e.target.value,
    })
  }
  clearInput = () => {
    this.setState({
      info: '',
    })
  }
  render() {
    return (
      <div>
        <div>{this.state.info}</div>
        <input type='text' onChange={this.handleChange} />
      </div>
    )
  }
}
export default Son;
```

> 值得注意的是，我们必须通过 `this.childRef.current`才能拿到子组件的实例。
> 使用 ref 常见的场景有管理焦点，文本选择或媒体播放、触发强制动画、集成第三方 DOM 库等。



## 生命周期

![react-16.4.png](2021-09-16- 【React】从Vue迈到React.assets/172968b85b8b028d~tplv-t2oaga2asx-watermark.image)

## Context

Context 设计目的是为了共享那些对于一个组件树而言是“全局”的数据，例如当前认证的用户、主题或首选语言。举个例子，在下面的代码中，我们通过一个 “theme” 属性手动调整一个按钮组件的样式：

```react
class App extends React.Component {
  render() {
    return <Toolbar theme="dark" />;
  }
}

function Toolbar(props) {
  // Toolbar 组件接受一个额外的“theme”属性，然后传递给 ThemedButton 组件。  
  // 如果应用中每一个单独的按钮都需要知道 theme 的值，这会是件很麻烦的事，  
  // 因为必须将这个值层层传递所有组件。  
  return (   
    <div>
      <ThemedButton theme={props.theme} />
    </div>  );
}

class ThemedButton extends React.Component {
  render() {
    return <Button theme={this.props.theme} />;
  }
}
```



使用 context, 我们可以避免通过中间元素传递 props：

`this.context` 与`React.createContext('值')`

```react
// Context 可以让我们无须明确地传遍每一个组件，就能将值深入传递进组件树。
// 为当前的 theme 创建一个 context（“light”为默认值）。

const ThemeContext = React.createContext('light');
class App extends React.Component {
  render() {
    // 使用一个 Provider 来将当前的 theme 传递给以下的组件树。    
    // 无论多深，任何组件都能读取这个值。    
    // 在这个例子中，我们将 “dark” 作为当前的值传递下去。    
    return (
      <ThemeContext.Provider value="dark">       
      		<Toolbar />
      </ThemeContext.Provider>
    );
  }
}

// 中间的组件再也不必指明往下传递 theme 了。
function Toolbar() {  
  return (
    <div>
      <ThemedButton />
    </div>
  );
}

class ThemedButton extends React.Component {
  // 指定 contextType 读取当前的 theme context。  
  // React 会往上找到最近的 theme Provider，然后使用它的值。  
  // 在这个例子中，当前的 theme 值为 “dark”。  
  static contextType = ThemeContext;
  render() {
    return <Button theme={this.context} />;  }
}
```



# 插件安装

`sass-loader和sass`

```json
"sass": "^1.43.4",
"sass-loader": "^8.0.2",
```

一个能跑得起来的版本

### 错误：TypeError: this.getOptions is not a function

解决：更换sass-loader版本

`react-router-dom`



# react-router

在打包之前,在package.json中private下(位置任意)添加"homepage": "./"

这样在你点击index.html时候路径没有问题不会报错,但是页面可能还是空白的

然后将项目中的引用路由时候的BrowserRouter改为HashRouter,所有用到的地方都要改

然后执行yarn build 或者 npm run build

再点开index.html就不会报错

原因：
        你必须把build里的文件直接放到应用服务器的根路径下，比如，你的服务器IP是172.16.38.253，应用服务器端口为8080，你应该保证http://172.16.38.253:8080这种访问方式，访问到的是你的build下的文件。如果你希望以http://172.16.38.253:8080/build/index.htm这种方式访问应用，那么你可以在package.json文件中增加一个homepage字段。

在服务器下运行和直接打开html文件有什么区别？
       最直接的区别，很容易注意到，一个是file协议，另一个是http协议。file协议更多的是将该请求视为一个本地资源访问请求，和你使用资源管理器打开是一样的，是纯粹的请求本地文件。而http请求方式则是通过假架设一个web服务器，解析http协议的请求然后向浏览器返回资源信息。我们所开发的html文件最后必定是会以网页的形式部署在服务器上，通过http协议访问，所以我们开发中也尽可能模拟线上环境，架设本地服务器，来避免file协议与http协议实现过程中的某些差异性，如某些API的差异、跨域请求的差异等。举个最容易验证的例子：在页面引入一张绝对路径的图片，即'/image/example.png'，然后分别通过这两种方式打开页面，file协议会将资源请求到根路径，而http协议虽然也会请求到根路径，但是是相对本地架设的服务器的根路径，一般也就是项目文件夹的路径。

        html是运行于客户端的超文本语言，从安全性上来讲，服务端不能对客户端进行本地操作。即使有一些象cookie这类的本地操作，也是需要进行安全级别设置的。

# 组件样式管理——没有Vue的scoped

使用webpack配置css-loader 使用 .module.css 的模块化 实现和Vue的scoped一样的hash

### CSS Modules

CSS Modules 指的是我们像 import js 一样去引入我们的 css 代码，代码中的每一个类名都是引入对象的一个属性，通过这种方式，即可在使用时明确指定所引用的 css 样式。

并且 CSS Modules 在打包的时候会自动将类名转换成 hash 值，完全杜绝 css 类名冲突的问题。

使用方式如下：

1、定义 css 文件。

```css
.className {
  color: green;
}
/* 编写全局样式 */
:global(.className) {
  color: red;
}

/* 样式复用 */
.otherClassName {
  composes: className;
  color: yellow;
}

.otherClassName {
  composes: className from "./style.css";
}
```

2、在 js 模块中导入 css 文件。

```js
import styles from "./style.css";

element.innerHTML = '<div class="' + styles.className + '">';
```

3、配置 css-loader 打包。

CSS Modules 不能直接使用，而是需要进行打包，一般通过配置 css-loader 中的 modules 属性即可完成 css modules 的配置。

```js
// webpack.config.js
module.exports = {
  module: {
    rules: [
      {
        test: /\.css$/,
        use:{
          loader: 'css-loader',
          options: {
            modules: {
              // 自定义 hash 名称
              localIdentName: '[path][name]__[local]--[hash:base64:5]',
            }
          }
       }
    ]
  }
};
```

4、最终打包出来的 css 类名就是由一长串 hash 值生成。

```css
._2DHwuiHWMnKTOYG45T0x34 {
  color: red;
}

._10B-buq6_BEOTOl9urIjf8 {
  background-color: blue;
}
```

# redux

#### 5. 经典案例

这是一个redux的经典案例

- 定义reducer函数根据action的类型改变state
- actions 定义指令
- 通过createStore创建store
- 调用store.dispatch()发出修改state的命令

```react
import { createStore } from 'redux'

const reducer = (state = {count: 0}, action) => {
  switch (action.type){
    case 'INCREASE': return {count: state.count + 1};
    case 'DECREASE': return {count: state.count - 1};
    default: return state;
  }
}

const actions = {
  increase: () => ({type: 'INCREASE'}),
  decrease: () => ({type: 'DECREASE'})
}

const store = createStore(reducer);

store.subscribe(() =>
  console.log(store.getState())
);

store.dispatch(actions.increase()) // {count: 1}
store.dispatch(actions.increase()) // {count: 2}
store.dispatch(actions.increase()) // {count: 3}

```

**我们可以直接在react component上使用store.dispatch，但是这样不太方便，这个时候我们需要react-redux**

```react
class Todos extends Component {
    render(){
        return(
            <div onCLick={()=>store.dispatch(actions.delTodo()) }>test</div>
        )
    }
}
```

#### 6. react-redux

Redux 官方提供的 React 绑定库。 具有高效且灵活的特性。

6.1 安装

```
npm install --save react-redux
复制代码
```

6.2 核心

- < Provider store>
- connect([mapStateToProps], [mapDispatchToProps], [mergeProps], [options])

Provider 内的任何一个组件（比如这里的 Comp），如果需要使用 state 中的数据，就必须是「被 connect 过的」组件——使用 connect 方法对「你编写的组件（MyComp）」进行包装后的产物。

这个函数允许我们将 store 中的数据作为 props 绑定到组件上。

简单的流程如下图所示：

![img](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/95e279721bec4ec4a9f3314a470d2f9c~tplv-k3u1fbpfcp-watermark.awebp)

react-redux中的connect方法将store上的getState 和 dispatch 包装成组件的props。

将之前直接在组件上dispatch的代码修改为如下：

index.js

```react
import React, { Component } from 'react';
import store from '../store';
import actions from '../store/actions/list';
import {connect} from 'react-redux';

class Todos extends Component {
    render(){
        return(
            <div onCLick={()=>this.props.del_todo() }>test</div>
        )
    }
}

export default connect(
    state=>state,
    actions
)(Todos);
```

Provider 能拿到关键的store并传递给每个子组件

#### 7. connect如何工作的？

connect() 接收四个参数，它们分别是 mapStateToProps ， mapDispatchToProps， mergeProps 和 options 。

7.1 `mapStateToProps这个函数允许我们将 store 中的数据作为 props 绑定到组件上。`

**reducer.js**

```react
export default function (state = { lists: [{text:'移动端计划'}],newType:'all'}, action) {
    switch (action.type) {
        case types.ADD_TODO:
            return {...state,lists:[...state.lists,{text:action.text}]}
        case types.TOGGLE_TODO:
            return {...state,lists:state.lists.map((item,index)=>{
                if(index == action.index){
                    item.completed = !item.completed
                }
                return item
            })}
        case types.DEL_TODO:
            return {...state,lists:[...state.lists.slice(0,action.index),...state.lists.slice(action.index+1)]}
        case types.SWITCH_TYPE:
            console.log({...state,newType:action.newType})
            return {...state,newType:action.newType}
        default:
            return state;
    }
}
```

在reducer.js中，定义了初始化的state，通过connect方法，我们就能使用this.props.lists拿到初始化的state。

```react
import React, { Component } from 'react';
import store from '../store';
import actions from '../store/actions/list';
import {connect} from 'react-redux';

class Todos extends Component {
    render(){
        return(
            {
                + <ul>
                +    this.props.state.lists.map(list =>(
                +        <li>{list.text}</li>
                +    ))
                + </ul>   
            }
            <div onCLick={()=>this.props.del_todo() }>test</div>
        )
    }
}

export default connect(
    state=>state,
    actions
)(Todos);
```

当 state 变化，或者 ownProps 变化的时候，mapStateToProps 都会被调用，计算出一个新的 stateProps，（在与 ownProps merge 后）更新给 MyComp。

7.2 `mapDispatchToProps(dispatch, ownProps): dispatchProps connect 的第二个参数是 mapDispatchToProps，它的功能是，将 action 作为 props 绑定到 MyComp 上。`

**action.js**

```react
import * as types from "../action-types";

export default{
    add_todo(text){
        return { type: types.ADD_TODO, text: text}
    },
    del_todo(idx){
        return {type:types.DEL_TODO, index: idx}
    },
    toggle_todo(index){
        return {type:types.TOGGLE_TODO, index}
    },
    del_todo(index){
        return {type:types.DEL_TODO, index}
    },
    switch_type(newType){
        return {type:types.SWITCH_TYPE, newType}
    }
}
```

我在action.js中定义的修改状态的命令，会通过connect 的 mapDispatchToProps方法变为props绑定在reac组件上。

我们可以方便得使用去调用

```react
    <div onCLick={()=>this.props.del_todo() }>test</div>
复制代码
```

#### 8. 深入

了解到这里，我们会发现并没有使用store.dispatch方法去发出命令，但是state已经修改，view也变化了，那么到底发生了什么？

```react
store.dispatch(actions.increase())
```

## HOOKS

前言
redux的经典使用方式就是通过高阶组件connect来将组件和store关联起来，我们见过最多的场景也是connect和类组件的组合使用（也可以和函数组件配合使用）。但在拥抱hooks的今天，我们如何抛弃connect，通过hooks获取到状态和dispatch呢？

react-redux中的hook
在react-redux7.1版本之后（如果你的版本比较低又不能升级版本，可以忽略本文）正式推出了三个hook: useDispatch, useSelector和useStore。这些hooks分别用于获取dispatch， 获取状态和获取store对象。接下来我们来对比一下之前的写法和hook的写法。

### connect版

```react
import actions from 'xxx';
   class Foo extends React.Component {
   componentDidMount() {
   	const { dispatch } = this.props;
   	dispatch(getData());		// 请求数据
   }

   renderData() {
   	const { data } = this.props; // 获取数据
   	return (
   		...
   	)
   }

   render() {
   	<div>
   		{ this.renderData() }
   	</div>
   }
   }

function mapStateToProps(state) {
    return { data: state.data };
}

export default connect(
	mapStateToProps, 			 // state
	{ getData: actions.getData } // actions
)(Foo);
```




### hook版

本示例中只用到了useSelector和useDispatch，因为这两个是在组件中最常用的，useStore则要在你需要使用store对象的地方使用。



```react
import actions from 'xxx';
import React, { useEffect } from 'react';
import { useDispatch, useSelector, shallowEqual } from 'react-redux';

function Foo(props) {
	const dispatch = useDispatch();
	const data = useSelector(state => state.data, shallowEqual);
useEffect(() => {
	dispatch(actions.getData());
}, [ dispatch ]);

const renderData = () => {
	return (
		...
	);
};

return (
	<div>
		{ renderData() }
	</div>
	)
}

export default Foo;
```


useStore的使用方法

```react
import { useStore } from 'react-redux';

function Foo(props) {
	const store = useStore();
	const state = store.getState();
	...
}
```





注意
useSelector接收两个参数： selector和equalityFn。第一个参数就像上面示例中一样，传入一个函数以获取你想要的state。第二个参数也是个函数，用于比较状态是否发生改变，如果这个函数的执行结果为false就表示状态发生改变，组件将重新渲染。useSelector源码是这样的：

function useSelector(selector, equalityFn = refEquality) {
 	invariant(selector, `You must pass a selector to useSelectors`);
 	
```react
const { store, subscription: contextSub } = useReduxContext();
 
return useSelectorWithStoreAndSubscription(
    selector,
    equalityFn,
    store,
    contextSub
)
```

我们可以看到有个默认值refEquality, 这个函数非常简单：

`const refEquality = (a, b) => a === b`

这个函数你可以自定义，react-redux也提供了一个现成的浅比较方法shallowEqual，这个就和connect比较state的方式很类似了，你可以这么用：

`import { useSelector, shallowEqual } from 'react-redux';
const data = useSelector(state => state.data, shallowEqual);`



官方的实用

```react
const init = { dataList:[] };//初始化dataState

const [state,dispatch] = useReducer(reducer,init);

useEffect(() => {
    //发送网络请求
    axios.get("/user?ID=123").then(res => {
        dispatch({
            type:ACTION_TYPE.GETDATA,
            payload: //后端返回的数据
        })
    })
},[]);

//使用 state.dataList 渲染页面
//在reducer页面

function reducer(state,action){
    switch(action.type){
        case "getData":
            return { //根据需求对数据做出操作，这里将返回的数据与初始化的数据合并
                ...state,
                dataList:action.payload 
            };
}    

export default reducer;
```




