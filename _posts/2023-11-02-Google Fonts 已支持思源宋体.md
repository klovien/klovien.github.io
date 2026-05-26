---
layout:     post
title:     Google Fonts 支持思源宋体
subtitle:   字体之美，大爱宋体
date:       2023-11-02
author:     ajiao
header-img: img/home-bg.jpg
catalog:   true
tags:
    - 学习
---
Google Fonts 分别在 11 月 18 日和 12 月 07 日提供了思源黑体和思源宋体的简繁支持，而且高达 6 种字重支持，其中思源宋体更是高达 7 种字重。这可了不得啊！更重要的是：它支持了目前电子显示屏上稀缺的宋体，这将会是一个伟大的进步！要知道，对于中文书籍，宋体一直是正文印刷的标准字体，而不是目前电子显示屏上普遍的黑体，因为宋体的衬线更适合长时间阅读。

## 优势

为什么宋体的衬线更适合长时间的阅读？可以有个「主观解释」，即自己可直接体会的解释。实践起来也简单，阅读一会儿宋体后回到黑体，阅读一会儿黑体后再回到宋体。

我自己的主观感受：看完宋体后再回到黑体，四字就浮上心头——索然无味。因此，作为长文章为主、文字内容为主的博客，强烈建议马上使用 Google Fonts 提供的思源宋体！你可能注意到，我的博客已经开始使用了，所以应该如何使用呢？

## 使用

官网搜索 Noto Serif SC，点 + 号选择，选择好后底部会弹出一个提示框，里面有使用说明。还可以点击提示框中的 CUSTOMIZE 定制要加载的字重与语言。

![\[1\]google-fonts-customize.png](https://io-oi.me/images/google-fonts-customize.png)
◎ 定制选项

之后，点击 EMBED，复制生成的代码，添加到博客的 <head> 标签内，并修改博客的相关 CSS 样式，指定 font-family 和 font-weight。

最后，考虑到宋体的笔画要比黑体细，因此建议将字体的颜色加深，比如修改为 #333，以达到较好的阅读效果。

此外，虽然之前 Adobe Typekit 就已经提供了类似的服务，但比起 Google Fonts，应用起来略显麻烦。

## 英文

对于中文，思源宋体已经非常美观了，但对于英文，我还是最喜欢 Garamond！

在已选择 Noto Serif SC 的基础上，继续搜索 EB Garamond，点 + 号选择，然后 CUSTOMIZE 勾选四个：

> regular 400 regular 400 Italic bold 700 bold 700 Italic

同样，复制生成的代码，添加到博客的 <head> 标签内，然后需修改 font-family：

```
font-family: 'EB Garamond', 'Noto Serif SC', serif;
```

## 福利

对于和我一样的 Hexo 的 NexT 主题使用者，按下列步骤操作。

1）_config.yml

```
# 文件位置：~/blog/themes/next/_config.yml

font:
- enable: false
+ enable: true

  # Global font settings used for all elements in <body>.
  global:
    external: true
+   family: EB Garamond
```

2）base.styl

```
# 文件位置：~/blog/themes/next/source/css/_variables/base.styl

// Font families.
-$font-family-chinese      = "PingFang SC", "Microsoft YaHei"
+$font-family-chinese      = "Noto Serif SC"
```

3）external-fonts.swig

```
文件位置：~/blog/themes/next/layout/_partials/head/external-fonts.swig
```

将这个文件的全部内容直接替换为 Google Fonts 网站生成的 <link> 代码，然后可以将 googleapis.com 修改为 loli.net。

4）custom.styl

```
/* 文件位置：~/blog/themes/next/source/css/_custom/custom.styl */

.post-body {
    color: #333;
}
```

最后，部署，完成！😆

## 链接

[衬线体的进化：从纸面到屏幕](https://zhuanlan.zhihu.com/p/49470735)

