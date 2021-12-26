---
layout:     post
title:      【React】React-router-dom
subtitle:   
date:       2021-11-03
author:     YeanSe
header-img: 
catalog: true
tags:
    - React
---

# React Router 入门完全指南(包含 Router Hooks)🛵

众所周知，React 是一个用于构建用户界面的 JavaScript 库,它是单页面应用(SPA). 单页面应用,顾名思义:只有一个页面，它是没有路由导航机制的. 这时候往往需要一种路由机制，以便在不同的视图之间切换而不用刷新整个网页. [React-Router](https://link.juejin.cn/?target=https%3A%2F%2Freactrouter.com%2Fcore%2Fapi%2Froute%2Froute-render-methods) 就是一个扩展 React 从而实现多页面跳转的第三方库。

在本教程中，将介绍 React Router 入门所需要知道的一切。

- React Router 入门完全指南(包含 Router Hooks)🛵
  - [React 项目初始化](https://juejin.cn/post/6948226424427773983#react-项目初始化)
  - [什么是路由?](https://juejin.cn/post/6948226424427773983#什么是路由)
  - [配置路由](https://juejin.cn/post/6948226424427773983#配置路由)
  - [渲染路由](https://juejin.cn/post/6948226424427773983#渲染路由)
  - [使用 Link 进行页面跳转](https://juejin.cn/post/6948226424427773983#使用-link-进行页面跳转)
  - [在路由中传递参数](https://juejin.cn/post/6948226424427773983#在路由中传递参数)
  - [使用 JS 代码实现页面跳转](https://juejin.cn/post/6948226424427773983#使用-js-代码实现页面跳转)
  - [重定向到其他页面](https://juejin.cn/post/6948226424427773983#重定向到其他页面)
  - [重定向到 404 页面](https://juejin.cn/post/6948226424427773983#重定向到-404-页面)
  - [路由守卫](https://juejin.cn/post/6948226424427773983#路由守卫)
  - Router Hooks
    - [useHistory](https://juejin.cn/post/6948226424427773983#usehistory)
    - [useParams](https://juejin.cn/post/6948226424427773983#useparams)
    - [useLocation](https://juejin.cn/post/6948226424427773983#uselocation)
  - [最后](https://juejin.cn/post/6948226424427773983#最后)
  - [参考文档](https://juejin.cn/post/6948226424427773983#参考文档)

## React 项目初始化

首先我们通过 create-react-app 初始化一个 React 应用

```shell
npx create-react-app react-router-guide
复制代码
```

然后添加下面的代码到`App.js`文件中

```tsx
import React from "react";
import "./index.css";

export default function App() {
  return (
    <main>
      <nav>
        <ul>
          <li>
            <a href="/">Home</a>
          </li>
          <li>
            <a href="/about">About</a>
          </li>
          <li>
            <a href="/contact">Contact</a>
          </li>
        </ul>
      </nav>
    </main>
  );
}
// Home Page
const Home = () => (
  <Fragment>
    <h1>Home</h1>
    <FakeText />
  </Fragment>
);
// About Page
const About = () => (
  <Fragment>
    <h1>About</h1>
    <FakeText />
  </Fragment>
);
// Contact Page
const Contact = () => (
  <Fragment>
    <h1>Contact</h1>
    <FakeText />
  </Fragment>
);

const FakeText = () => (
  <p>
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod
    tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
    quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
    consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
    cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat
    non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
  </p>
);
复制代码
```

现在如果你准备好了，那就让我们理解一个重要的问题: 什么是路由?

## 什么是路由?

路由是一种向用户显示不同页面的能力。 这意味着用户可以通过输入 URL 或单击页面元素在 WEB 应用的不同部分之间切换。

安装`react-router-dom`:

```shell
yarn add react-router-dom
复制代码
```

这里需要说明一下 React Router 库中几个不同的 npm 依赖包，每个包都有不同的用途

- `react-router`: 实现了路由的核心功能，用作下面几个包的运行时依赖项(peer dependency)。
- `react-router-dom`: 用于 React WEB 应用的路由依赖. 基于 react-router，加入了在浏览器运行环境下的一些功能，例如：`BrowserRouter` 和 `HashRouter` 组件，前者使用 `pushState` 和 `popState` 事件构建路由;后者使用 `window.location.hash` 和 `hashchange` 事件构建路由
- `react-router-native`: 用于 React Native 应用的路由依赖。基于 react-router，加入了 react-native 运行环境下的一些功能
- `react-router-config`: 用于配置静态路由的工具库

结论:

`react-router-dom` 依赖 `react-router`，所以我们使用 npm 安装依赖的时候，只需要安装相应环境下的库即可，不用再显式安装 react-router。npm 会自动解析 `react-router-dom` 包中 package.json 的依赖并安装。

基于浏览器环境的开发，只需要安装 `react-router-dom`; 基于 react-native 环境的开发，只需要安装 `react-router-native`;

现在，我们已经成功安装了路由库！接下来，让我们进入使用阶段

![庆祝](https://media.giphy.com/media/12UlfHpF05ielO/source.gif)

## 配置路由

为了使安装的的路由库在 React 应用中可以使用，首先需要从`react-router-dom`中导入`BrowserRouter`

在`App.js`文件中输入以下代码

```ts
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router } from "react-router-dom";

export default function App() {
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <a href="/">Home</a>
            </li>
            <li>
              <a href="/about">About</a>
            </li>
            <li>
              <a href="/contact">Contact</a>
            </li>
          </ul>
        </nav>
      </main>
    </Router>
  );
}
复制代码
```

如果我们需要导航到应用中所需要的任意页面，我们必须使用 `BrowserRouter` 作为最顶层组件包裹其他组件。

`Router`本身并不能实现页面跳转，因为我们还没有配置路径与页面组件的映射关系。 下面让我们开始添加这个关系。

## 渲染路由

为了渲染出路由，我们需要导入 `Route` 组件

修改 `App.js`为以下内容

```tsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route } from "react-router-dom";

export default function App() {
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <a href="/">Home</a>
            </li>
            <li>
              <a href="/about">About</a>
            </li>
            <li>
              <a href="/contact">Contact</a>
            </li>
          </ul>
        </nav>
        <Route path="/" render={() => <h1>Welcome!</h1>} />
      </main>
    </Router>
  );
}
复制代码
```

现在我们添加了一条路由`<Route path="/" render={() => <h1>Welcome!</h1>} />`

`Route`组件有很多属性，在代码中，我们使用了 `path`, `render`属性

- `path`:页面的路径.在上面的代码中，我们定义了`/`路径用于导航到首页
- `render`: path 对应的页面渲染的是什么.在代码中，我们渲染了一个`h1`标题

那么，我们怎么渲染 React 组件呢？那就要说到`Route` 组件的另一个属性`component`

让我们更新`App.js`为以下内容

```tsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route } from "react-router-dom";

export default function App() {
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <a href="/">Home</a>
            </li>
            <li>
              <a href="/about">About</a>
            </li>
            <li>
              <a href="/contact">Contact</a>
            </li>
          </ul>
        </nav>

        <Route path="/" component={Home} />
      </main>
    </Router>
  );
}

const Home = () => (
  <Fragment>
    <h1>Home</h1>
    <FakeText />
  </Fragment>
);
复制代码
```

我们把 `render`属性替换为 `component`就可以渲染我们的`Home`组件了

在真实的开发中，前端页面当然不止一个，我们肯定需要建立多个页面与路由进行映射，然后进行页面之间的跳转

## 使用 Link 进行页面跳转

使用`Link`组件实现页面之间的跳转，让我们更新`App.js`为以下内容

```tsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route, Link } from "react-router-dom";

export default function App() {
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to="/about">About</Link>
            </li>
            <li>
              <Link to="/contact">Contact</Link>
            </li>
          </ul>
        </nav>

        <Route path="/" exact component={Home} />
        <Route path="/about" component={About} />
        <Route path="/contact" component={Contact} />
      </main>
    </Router>
  );
}

const Home = () => (
  <Fragment>
    <h1>Home</h1>
    <FakeText />
  </Fragment>
);

const About = () => (
  <Fragment>
    <h1>About</h1>
    <FakeText />
  </Fragment>
);

const Contact = () => (
  <Fragment>
    <h1>Contact</h1>
    <FakeText />
  </Fragment>
);
复制代码
```

导入 `Link`之后，我们还需要修改导航菜单部分的代码-使用`Link`及其`to`替换`a`及其`href` ,这样就可以实现界面之间的跳转功能，并且界面不会重新刷新

接下来，还要多添加几个页面以验证我们的路由跳转功能是否正常。在代码中，我们添两个了路由组件`About` 和 `Contact`

现在，我们可以在单页面应用中实现界面的跳转了，但是还会有一个问题: 不管我们跳转哪个界面，`Home`组件都会渲染

这是因为在 React Router 中，如果定义的`path`属性以`/`开头，那么每次都会匹配到`Home`组件

如何解决呢?

只需要给`Home` 路由加上`exact`,这样只有`path`的值被完全匹配时才会渲染对应的组件

还可以通过使用`Switch`包裹路由来告诉 React Router 一次仅加载一条路由。

```tsx
import { BrowserRouter as Router, Route, Link, Switch } from "react-router-dom";

<Switch>
  <Route path="/" exact component={Home} />
  <Route path="/about" component={About} />
  <Route path="/contact" component={Contact} />
</Switch>;
复制代码
```

## 在路由中传递参数

更新`App.js`为以下内容

```jsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route, Link, Switch } from "react-router-dom";

export default function App() {
  const name = "John Doe";
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to={`/about/${name}`}>About</Link>
            </li>
            <li>
              <Link to="/contact">Contact</Link>
            </li>
          </ul>
        </nav>
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/about/:name" component={About} />
          <Route path="/contact" component={Contact} />
        </Switch>
      </main>
    </Router>
  );
}

const Home = () => (
  <Fragment>
    <h1>Home</h1>
    <FakeText />
  </Fragment>
);

const About = ({
  match: {
    params: { name },
  },
}) => (
  // props.match.params.name
  <Fragment>
    <h1>About {name}</h1>
    <FakeText />
  </Fragment>
);

const Contact = () => (
  <Fragment>
    <h1>Contact</h1>
    <FakeText />
  </Fragment>
);
复制代码
```

正如你看到的，我们定义了一个常量 `name`，然后把它作为跳转到`About`页面的参数

然后，我们还必须调整`About`路由的路径，以便让页面能够接受到`name`参数-`path ="/about/:name"`

接下来`About`组件就可以通过`props.match.params.name`接受到路由传递过来的参数

到目前为止，我们已经做了很多工作。 但是在某些情况下，我们不想使用`Link`在页面之间导航。因为有时我们需要等待某些操作完成后自动跳转到下一页.下面让我们来看看如何实现这个逻辑

## 使用 JS 代码实现页面跳转

通过`Route`作为顶层组件包裹其他组件后,页面组件就可以接收到一些路由相关的东西，比如`props.history`

```jsx
const Contact = ({ history }) => (
  <Fragment>
    <h1>Contact</h1>
    <button onClick={() => history.push("/")}>Go to home</button>
    <FakeText />
  </Fragment>
);
复制代码
```

props 中接收到的`history`对象具有一些方便的方法，如`goBack`，`goForward`,`push`等。在代码中，我们使用 `push` 方法跳转到主页。

接下来让我们处理在页面跳转后想要重定向到其他页面的情况。

## 重定向到其他页面

React Router 有一个 `Redirect`组件，就是字面意思，它可以帮助我们实现页面重定向

继续修改`App.js`文件

```jsx
import {
  BrowserRouter as Router,
  Route,
  Link,
  Switch,
  Redirect,
} from "react-router-dom";

const About = ({
  match: {
    params: { name },
  },
}) => (
  // props.match.params.name
  <Fragment>
    {name !== "tom" ? <Redirect to="/" /> : null}
    <h1>About {name}</h1>
    <FakeText />
  </Fragment>
);
复制代码
```

当接收到的路由参数`name` 不等于 `tom` 的时候，将会自动重定向到首页

你可能也会想到，还可以使用`props.history.push("/")`实现这个功能,这里我们说明一下

- `Redirect`组件将替换当前页面，因此用户无法返回上一页;
- 使用 push 方法，用户是可以返回上一页的。

但是，你还可以使用`props.history.replace('/')`来模仿重定向行为。

接下来让我们继续处理当用户访问一条不存在的路由时的情况。

## 重定向到 404 页面

重定向到 404 页面,你可以使用`Route`组件的`component`属性来显示 404 页面。但是在这里为了演示简便，我们使用`Route`组件的`render`属性即可

```tsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route, Link, Switch } from "react-router-dom";

export default function App() {
  const name = "John Doe";

  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to={`/about/${name}`}>About</Link>
            </li>
            <li>
              <Link to="/contact">Contact</Link>
            </li>
          </ul>
        </nav>
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/about/:name" component={About} />
          <Route path="/contact" component={Contact} />
          {/*404 page*/}
          <Route render={() => <h1>404: page not found</h1>} />
        </Switch>
      </main>
    </Router>
  );
}
复制代码
```

我们添加了一条新路由，当用户访问不存在的路径时，将会匹配到此路由并重定向到 404 页面。

让我们继续前进，在下一部分中学习如何建立路由守卫(路由权限验证)。

## 路由守卫

```tsx
import React, { Fragment } from "react";
import "./index.css";

import { BrowserRouter as Router, Route, Link, Switch } from "react-router-dom";

export default function App() {
  const name = "John Doe";
  const isAuthenticated = false;
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to={`/about/${name}`}>About</Link>
            </li>
            <li>
              <Link to="/contact">Contact</Link>
            </li>
          </ul>
        </nav>
        <Switch>
          <Route path="/" exact component={Home} />
          {isAuthenticated ? (
            <>
              <Route path="/about/:name" component={About} />
              <Route path="/contact" component={Contact} />
            </>
          ) : (
            <Redirect to="/" />
          )}
        </Switch>
      </main>
    </Router>
  );
}
复制代码
```

正如你所看到的，我们声明了一个模仿身份验证的变量`isAuthenticated`。 然后，检查用户是否已通过身份验证。 如果通过验证，则渲染受保护的页面。 否则，将重定向到主页。

到目前为止，我们已经介绍了很多内容，但还有一个有趣的部分：`router hooks`

让我们进入最后一部分-Hooks。

![加油](https://media.giphy.com/media/nXxOjZrbnbRxS/source.gif)

## Router Hooks

Router hooks 可以让我们更加容易地访问到 `history`,`location`,路由参数 等等

### useHistory

`useHistory` 帮助我们直接访问到`history`,而不再需要通过 props 访问

```tsx
import { useHistory } from "react-router-dom";

const Contact = () => {
  const history = useHistory();
  return (
    <Fragment>
      <h1>Contact</h1>
      <button onClick={() => history.push("/")}>Go to home</button>
    </Fragment>
  );
};
复制代码
```

### useParams

`useParams` 帮助我们直接访问到路由参数,而不再需要通过 props 访问

```tsx
import {
  BrowserRouter as Router,
  Route,
  Link,
  Switch,
  useParams,
} from "react-router-dom";

export default function App() {
  const name = "John Doe";
  return (
    <Router>
      <main>
        <nav>
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to={`/about/${name}`}>About</Link>
            </li>
          </ul>
        </nav>
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/about/:name" component={About} />
        </Switch>
      </main>
    </Router>
  );
}

const About = () => {
  const { name } = useParams();
  return (
    // props.match.params.name
    <Fragment>
      {name !== "John Doe" ? <Redirect to="/" /> : null}
      <h1>About {name}</h1>
      <Route component={Contact} />
    </Fragment>
  );
};
复制代码
```

### useLocation

`useLocation` 会返回当前 URL 的 location 对象

```tsx
import { useLocation } from "react-router-dom";

const Contact = () => {
  const { pathname } = useLocation();

  return (
    <Fragment>
      <h1>Contact</h1>
      <p>Current URL: {pathname}</p>
    </Fragment>
  );
};
复制代码
```

## 最后

React Router 是一个了不起的库，它可以在一个页面模拟出中多页面的情况，并具有很高的可用性。 （但是归根结底，它仍然是单页面应用）。

现在还有了 router hooks，你可以更加优雅和容易地实现页面的跳转。 在你的下一个项目中，绝对是需要考虑的因素。
