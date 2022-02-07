---
layout:     post
title:      处理 AndroidKiller 反编译失败问题
subtitle:   报错内容：AndroidKiller 反编译失败，无法继续下一步源码反编译!
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-universe.jpg
catalog: true
tags:
    - 安卓逆向
---


## 1. 问题

- 使用 Android Killer 进行 apk 文件反编译时，遇到“反编译失败，无法继续下一步源码反编译!” 报错。

> **报错内容：**
>当前 Apktool 使用版本：android 2.4.1
正在反编译 APK，请稍等...
>I: Using Apktool 2.4.1 on chelaile_app.apk
>I: Loading resource table...
>I: Decoding AndroidManifest.xml with resources...
>I: Loading resource table from file: C:\Users\Administrator\AppData\Local\apktool\framework\1.apk
>I: Regular manifest package...
>I: Decoding file-resources...
>I: Decoding values */* XMLs...
>I: Baksmaling classes.dex...
>I: Baksmaling classes2.dex...
>I: Baksmaling classes3.dex...
>I: Baksmaling classes4.dex...
>I: Baksmaling assets/A3AEECD8.dex...
>Exception in thread "main" org.jf.dexlib2.dexbacked.DexBackedDexFile$NotADexFile: Not a valid dex magic value: cf 77 4c c7 9b 21 01 cd
>	at org.jf.dexlib2.util.DexUtil.verifyDexHeader(DexUtil.java:93)
>	at org.jf.dexlib2.dexbacked.DexBackedDexFile.getVersion(DexBackedDexFile.java:111)
>	at org.jf.dexlib2.dexbacked.DexBackedDexFile.<init>(DexBackedDexFile.java:78)
>	at org.jf.dexlib2.dexbacked.DexBackedDexFile.<init>(DexBackedDexFile.java:138)
>	at org.jf.dexlib2.dexbacked.ZipDexContainer$1.getDexFile(ZipDexContainer.java:181)
>	at brut.androlib.src.SmaliDecoder.decode(SmaliDecoder.java:90)
>	at brut.androlib.src.SmaliDecoder.decode(SmaliDecoder.java:39)
>	at brut.androlib.Androlib.decodeSourcesSmali(Androlib.java:96)
>	at brut.androlib.ApkDecoder.decode(ApkDecoder.java:164)
>	at brut.apktool.Main.cmdDecode(Main.java:170)
>	at brut.apktool.Main.main(Main.java:76)
>APK 反编译失败，无法继续下一步源码反编译!


## 2. 更新 APKTOOL

- 下载地址：[https://connortumbleson.com/apktool/](https://connortumbleson.com/apktool/)，此处下载最新版本 **apktool_2.4.1.jar**；

>***注意：如果 APKTOOL 已经是最新版本，则直接跳到第三步！***

## 3. Frida安装配置

##### 电脑安装 frida & frida-tools

```
pip install frida
pip install frida-tools
```

> 注意：frida 版本与 python 版本之间的对应关系，可以参考：[https://pypi.org/project/frida/#files](https://pypi.org/project/frida/#files)。

##### 下载 frida-server

- 下载地址：[https://github.com/frida/frida/releases](https://github.com/frida/frida/releases)。

##### 手机运行 frida-server

```
adb push frida-server-15.1.14-android-x86 /data/local/tmp/

adb shell

cd /data/local/tmp

# 确定手机当前用户是root用户或拥有root权限
chmod 777 frida-server-15.1.14-android-x86

./frida-server-15.1.14-android-x86

```

##### 电脑上运行

```
frida-ps -U
```

- 如果运行成功，命令行窗口会打印出手机上正在运行的程序。



