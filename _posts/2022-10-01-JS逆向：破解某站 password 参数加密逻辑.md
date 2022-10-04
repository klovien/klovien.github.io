---
layout:     post
title:      JS逆向：破解某站 password 参数加密逻辑
subtitle:   
date:       2022-02-01
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - JS逆向
---

#### 目标站点：`aHR0cDovL2VpcC5jaGFuZmluZS5jb20vbG9naW4uanNw`

>说明： 该站点较简单，可以作为小白练手使用，方便理解 js 逆向常规步骤。

#1. 定位代码
- 打开调试面板，输入手机号、密码，点击登录，检查 Network，分析后发现箭头指向的请求，分析发现参数 `j_password` 已经被加密；

![]({{site.baseurl}}/img-post/password-1.png)
- 全局搜索 `j_password`，下断点，重新输入账号密码，网页在下图位置被断住，分析发现 `desEncrypt` 函数返回的结果就是被解密后的数据；

![]({{site.baseurl}}/img-post/password-2.png)
# 2. 分析调试
- 回溯，进入 `function desEncrypt(value, xForm, type)` 中，分析发现 `_0(xForm)` 结果为 `false`，在 `else` 代码 `keyObj = SECURITYKEY.get();` 处下断，如下图；

![]({{site.baseurl}}/img-post/password-3.png)
- 回溯分析 ` SECURITYKEY.get()` 函数，在代码 `str = SECURITYKEY._2();` 处下断，回溯分析 `SECURITYKEY._2()` 函数，发现该函数是通过 ajax 发送请求得到返回结果的，请求的 url 中包含 `js/session.jsp?_=`，并且还跟了一个时间戳后缀，通过搜索过滤请求，找到了目标 url： `http://eip.chanfine.com/resource/js/session.jsp?_=1637761979345&s_ajax=true`，该 url 尾部也跟了一个时间戳；

![]({{site.baseurl}}/img-post/password-4.png)
- 访问上面拿到的 url，得到如下返回值，正是我们想要的。
```
function getSessionId(){
	return "3B9B93AF595C0F909356C09468D9F041";
}
```
- 回退，继续单步调试，发现 `encodeType == null || encodeType == 'aes'` 结果为 `true`，此处 else 条件分支可以忽略。单步调试后发现，前述的返回值被分割成多个部分并赋值给了一个对象 key，最后返回了一个对象；

![]({{site.baseurl}}/img-post/password-5.png)

- 再次回退，分析 `CryptoJS.AES.encrypt(value, CryptoJS.enc.Utf8.parse(keyObj.key)`，

**分析发现**：
-- `keyObj.key` 就是前面得到的 `key` 对象的 `key` 属性值，
-- `CryptoJS.enc.Utf8.parse()` 函数对应的函数在122行：
```
parse: function(a) {
            return b.parse(unescape(encodeURIComponent(a)))
        }
```
-- 分析上面的代码，`encodeURIComponent` 是内生的自带函数，`unescape()` 也是内生的自带函数，而且两个函数都为对传入的参数 `a` 做任何修改。需要继续回溯分析的 `b.parse()` 函数，对应函数代码为：
```
parse: function(a) {
            for (var c = a.length, e = [], j = 0; j < c; j++)
                e[j >>> 2] |= (a.charCodeAt(j) & 255) << 24 - 8 * (j % 4);
            return new r.init(e,c)
        }
```
-- 分析上面的代码，发现其执行过程包括一个 for 循环，单步调试发现并不需要进一步回溯，需要回溯的是return 的 `new r.init(e,c)`，继续回溯得到如下代码：
```
init: function(a, c) {
            a = this.words = a || [];
            this.sigBytes = c != p ? c : 4 * a.length
        },
```
- 到这一步，其实已经没有必要继续在扣代码了，结合前面的上下文，可以知道整个js代码其实可以全部扣下来，直接进行调用的，因为 `value = CryptoJS.AES.encrypt(value, CryptoJS.enc.Utf8.parse(keyObj.key), {iv:CryptoJS.enc.Utf8.parse(keyObj.iv)}).toString()` 这段加密代码，全部是对 `CryptoJS` 的调用。

![]({{site.baseurl}}/img-post/password-5.png)
- 打开 console，输出前面分析的结果；
![]({{site.baseurl}}/img-post/password-5.png)

- 如上图，得到加密后的内容，至此调试分析结束。
# 3. js代码实现
- 通过上面的分析，知道核心加密步骤分为两步：
>第一步，是通过 `SECURITYKEY.get()` 生成一个对象，在此过程中还需要 ajax 发送一个带时间戳的请求；
第二步，是通过 `CryptoJS` 中的 `AES` 和 `enc` 进行加密处理；
- js 代码：
  略。