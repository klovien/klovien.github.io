---
layout:     post
title:      【React】什么情况下你该使用useCallback？
subtitle:   
date:       2022-01-21
author:     YeanSe
header-img: 
catalog: true
tags:
    - React
    - React-Native

---

每个函数都要使用一次useCallback，几乎是每个react小白必犯的一个错误。

小白时期总会认为useCallback缓存函数，不用再次生成是对性能的优化。但根据react官方，在函数组件中生成函数的时间几乎是微乎其微的，这也是使用hooks的基础。

然而使用useCallback恰恰是对性能的一种损耗，传递依赖数组来diff不同，diff也会带来一定的性能损耗。

diff 我的理解是 损失部分性能 来估计、挽回 一下整体性能的过程

这个是useMemo优化的地方，useCallback并非和useMemo一样。



那么什么时候使用useCallback呢，我认为

<u>**只有当一个函数的更新，会造成性能损失的时候。**</u>

<u>**只有当一个函数的更新，会造成性能损失的时候。**</u>

<u>**只有当一个函数的更新，会造成性能损失的时候。**</u>

<u>这时候使用useCallback</u>

` // eslint-disable-next-line react-hooks/exhaustive-deps`

换句话讲，useCallback不是在做无关紧要的性能的锦上添花的，而是在引导你正确使用hook的一个必需品。



# 当其他hook依赖数组中有函数时

```react
const [state,setState] = useState(false);
//抽离出两个useEffect中的共同的函数，需要用useCallback包裹
const stateChange = useCallback(() => {
  setState(!state);
}, [state]);

useEffect(() => {
  stateChange();
}, [stateChange]);

useEffect(() => {
  const callBack = () => {
    stateChange();
    //...
	};
	EventEmitter.addListener('changeState', callBack);
    return () => {
      EventEmitter.removeListener('changeState', callBack);
	};
}, [stateChange]);
```

1. 上述代码抽离了共同的函数，这样依赖数组更干净了，变成了一条依赖链更加简洁
2. 不会每次更新时都更新函数，导致useEffect的触发

# 当函数更新引起复杂组件的更新

和依赖数组同理，props其实也是一个依赖数组，也会导致 re更新

假设你props传了一个函数，这时也需要用useCallback

每次渲染，如果不用usecallback,那函数每次都会是一个新地址，每次用这个函数作为参数的组件都会重新渲染

