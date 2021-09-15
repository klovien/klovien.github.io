---
layout:     post
title:      【JavaScript】dom操作
subtitle:   
date:       2021-09-12
author:     YeanSe
header-img: 
catalog: true
tags:
    - JavaScript
---

# 1.临时绑定类名` element.className+=“ active”`

```javascript
/**
 * @func 添加临时的active类名 
 * @param {object} element 一个dom结点
 * @param { number | any} duration 持续时间
 */
 addActiveClass (element, className = "active", duration = 50) {
            if (element == undefined) return;
            let primary = element.className;
            if (!primary.includes(className)) {
                element.className = primary + " " + className;
                setTimeout(() => {
                    //这里面用定时器，尽量用箭头函数
                    element.className = priÏmary;
                }, delay);
            }
},
```

# 2.keyevent键盘事件 window 或 input

键盘事件要么在window上绑要么在input上绑，没有其他应用场景的可能和实现方法。如果有，到时候我再回来补充。

keydown keyup 一些功能键只能用keydown

keypress主要是大键盘的文本区，现在已经不用了

### 小案例

```javascript
/**
 * @func 给窗口绑定计算器的键盘事件
 */
export function addKeyActiveEvent() {
    console.log('绑定键盘事件已触发');
    let that = this;
    const keydown = debounce(function (event) {
        if (event.defaultPrevented) {
            return; // 如果事件已经在进行中，则不做任何事。
        }
        if (0 <= Number(event.key) && Number(event.key) <= 9) {
            that.append(Number(event.key))
            return;
        }
        // console.log(event.key);
        switch (event.key) {
            case "+":
            case "-":
            case ".":
                that.append(event.key)
                break;
            case "*":
                that.append('×');
                break;
            case "/":
                that.append('÷');
                break;
            case "Enter":
                that.calculate();
                break;
            case "Backspace":
                that.delete();
                break;
            case "Escape":
                that.quit();
                break;
            default:
                return; 
        }

        // 取消默认动作，从而避免处理两次。
        event.preventDefault();
    }, 50);
    window.addEventListener("keydown", keydown);
}
```

# 3.实现自动滚动到最底部 `div.scrollTop = div.scrollHeight;`

一个小案例

```javascript
<body>
    <div id="example">
        <h3 id="example_title">将滚动条（scrollbar）保持在最底部的方法</h3>
        <div id="example_main">
            <script type="text/javascript">
                function add()
                {
                    var now = new Date();
                    var div = document.getElementById('scrolldIV');
                    div.innerHTML = div.innerHTML + 'time_' + now.getTime() + '<br />';
                    div.scrollTop = div.scrollHeight;
                }
            </script>
            <span class="notice">请点击“插入一行”按钮，插入最新信息，当出现滚动条时，滚动条将自动保持在底部。</span><br />

            <div id="scrolldIV" style="overflow:auto; height: 100px; width: 400px; border: 1px solid #999;">
            </div>
            <input type="button" value="插入一行" onclick="add();">
        </div>
    </div>
</body>
```

