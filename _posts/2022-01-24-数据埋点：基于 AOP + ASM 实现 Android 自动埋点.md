---
layout:     post
title:      数据埋点：基于 AOP + ASM 实现 Android 自动埋点
subtitle:   
date:       2022-01-27
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - 数据埋点
---



#### ASM 是什么

- ASM 是字节码操作库，支持对字节码进行编辑，实现类、属性和方法的增删改查；
- 字节码操作库还有 Javaassit 库可以选择，但 ASM 灵活度和效率都是最高的；
- ASM 可以直接产生二进制的 class 文件，也可以增强既有类的功能。
  - Java class 被存储在严格格式定义的 .class文件里，这些类文件拥有足够的元数据来解析类中的所有元素，包括：
    - 类名称
    - 方法
    - 属性
    - Java 字节码（指令）。

#### ASM 字节码插桩

- 利用操作 ASM 字节码实现方法计时，可以的做法是修改 class 文件，在目标方法开始和结束时插入本来需要手动埋点的代码，这个过程称之为字节码插桩。

- Gradle Transform 

  ![]({{site.baseurl}}/img-post/数据埋点-8.png)

  - Gradle Transform 是 Android 官方提供给开发者在项目构建阶段即由 class 到 dex 转换期间修改 class 文件的一套 api。
    - 目前比较经典的应用是字节码插桩、代码注入技术。
    
- 上图所示是 Android 打包流程：.java 文件 -> .class 文件 -> .dex 文件；
- 只要在红圈处拦截住，拿到所有方法进行修改完再放生就可以了，而做到这一步也不难，Google 官方在Android Gradle 的 1.5.0 版本以后提供了 Transfrom API，允许第三方 Plugin 在打包 dex 文件之前的编译过程中操作 .class 文件；
- ASM 字节码插桩，要做的就是实现 Transform 进行 .class 文件遍历拿到所有方法，修改完成对原文件进行替换。

#### ASM 框架中的核心类

- ClassReader
  - 该类用来解析编译过的class字节码文件。

- ClassWriter
  - 该类用来重新构建编译后的类，比如说修改类名、属性以及方法，甚至可以生成新的类的字节码文件。
  
- ClassViitor
  - 主要负责 “拜访” 类成员信息，包括：
    - 标记在类上的注解，
    - 类的构造方法，
    - 类的字段，
    - 类的方法，
    - 静态代码块。

- AdviceAdapter
  - 实现了 MethodVisitor 接口，主要负责 “拜访” 方法的信息，用来进行具体的方法字节码操作。

  
#### ASM 插桩实现方案

- 方案简介
  - class 文件是按照 JVM 规范格式存储的二进制文件，本质上是一个表，记录了类的常量池、访问标志、属性和方法等。
  - ASM 库不仅能够对 class 文件进行解读，还提供了方便的 API 进行字节码的修改，支持直接产生二进制 class 文件。
  - ASM 提供了基于事件的 API：
    - ClassReader 用于读取 class 文件的二进制流；
    - ClassVisitor 以事件的形式输出 class 的结构信息；
    - ClassWriter 则用于把修改后的字节码生成二进制流。

- 示例：监听点击事件

- 步骤一：通过 visitMethod 拿到方法名，判断这个方法是不是要监控的方法

  - ASM 读取分析 class 文件
  
    - class 文件在工程的 `app/build/intermediates/classes` 目录下
      ```
      public static void main(String[] args) {  
          try {
              File classFile = new File("./source/MainActivity.class");
              File dir = new File(".");
              transformClassFile(dir, classFile)
          } catch (Exception e){}
      }
      private static File transformClassFile(File dir, File sourceFile){  
          String className = sourceFile.getName();
          // 得到class文件二进制流
          FileInputStream fileInputStream = new FileInputStream(sourceFile);
          byte[] sourceClassBytes = IOUtils.toByteArray(fileInputStream);
          // 定义classWriter,用于输出修改后的二进制流
          ClassWriter classWriter = new ClassWriter(ClassWriter.COMPUTE_MAXS);
          // 自定义ClassVisitor, 负责字节码的消费
          MyClassVisitor myClassVisitor = new MyClassVisitor(classWriter);
          // ClassReader负责字节码的读取
          ClassReader classReader = new ClassReader(sourceClassBytes);
          // 开始字节码处理
          classReader.accept(myClassVisitor, 0);
          // 生成二进制流并保存成新的文件
          byte[] destByte = classWriter.toByteArray();
          File modified = new File(dir, className)
          if (modified.exists()) {
              modified.delete()
          }
          modified.createNewFile();
          new FileOutputStream(modified).write(destByte)
          return modified;
      }
   
      private static class MyClassVisitor extends ClassVisitor {
   
          public MyClassVisitor(ClassVisitor classVisitor) {
              super(Opcodes.ASM6, classVisitor);
          }
          @Override
          public void visit(int version, int access, String name, String signature, String superName, String[] interfaces) {
              System.out.println("visit： access: " + access + " ,name: " + name + " , superName: " + superName + " ,singature: " + signature + ", interfaces: " + interfaces.join("/"));
                  super.visit(version, access, name, signature, superName, interfaces);
          }
          @Override
          public MethodVisitor visitMethod(int access, String name, String descriptor, String signature, String[] exceptions) {
              System.out.println("visitMethod： access: " + access + " ,name: " + name + " , desc: " + descriptor + " ,singature: " + signature);
              MethodVisitor mv = super.visitMethod(access, name, descriptor, signature, exceptions);
              MethodVisitor myMv = new MethodVisitor(Opcodes.ASM6, mv) {
                  @Override
                  AnnotationVisitor visitAnnotation(String desc, boolean visible) {
                      System.out.println("visitAnnotation: desc: " + desc);
                      return super.visitAnnotation(desc, visible)
                  }
                  @Override
                  void visitCode() {
                      super.visitCode()
                  }
              }
              return myMv;
          }
      }
      ```
    
  - 用 ClassReader 读取 class 文件，并用自定义的 ClassVisitor 接收事件，查看输出：
    ```
    visit： access: 33 ,name: com/example/wangkai/MainActivity , superName: android/support/v7/app/AppCompatActivity ,singature: null, interfaces:  
    visitMethod： access: 1 ,name: <init> , desc: ()V ,singature: null  
    visitMethod： access: 4 ,name: onCreate , desc: (Landroid/os/Bundle;)V ,singature: null  
    visitMethod： access: 1 ,name: fun , desc: ()V ,singature: null  
    visitAnnotation: desc: Lcom/example/wangkai/annotation/TraceTime;  
    ```
  - 这里通过 visit 回调可以读取到 class 的名字、父类名和接口，这样就可以判断出一个类是否是我们要插桩的白名单页面，是不是 Activity 子类以及是否实现了点击事件接口 View$onClickListener（实现对点击事件的监控）
  - 通过在 visitMethod 方法里返回自定义的 MethodVisitor 对象，拿到方法上的注解，从而可以知道这个方法是否是要插桩的方法
  - 上面的输出中，visitCode 表示方法开始执行，如果能在这里插入代码，那代码就能在原始代码执行前执行。
  - 我们已经找到了切入点，下一步就是插入代码了。

- 步骤二：插入埋点代码

  - 插入代码要难一些，因为我们是在字节码层面操作，插入的也只能是字节码，这就需要对字节码有一定了解。
  - 包括局部变量表和操作数栈的概念，常见指令（ALOAD， INVOKEVIRTUAL等）的含义。
  - 埋点时，我们需要插入这样的代码：

    ```
    private static class MyClickListener implements View.OnClickListener{  
    @Override
    public void onClick(View v) {
    ClickAgent.click(v); //待插入代码，方法里获取view的ID和当前时间，实现对点击事件的记录
    Log.d(TAG, "onClick: ");
      }
    }
    ```
  - 上面这步操作的是，通过 ASM methodVisitor 提供的 API，把 ClickAgent.click(v) 的字节码，注入到原始 onClick 方法里。

  - 查看字节码：
    ```
    L0  
    LINENUMBER 27 L0  
    ALOAD 1  
    INVOKESTATIC com/example/wangkai/ClickAgent.click (Landroid/view/View;)V  
    L1  
    LINENUMBER 28 L1  
    LDC "MainActivity"  
    LDC "onClick: "  
    ```
  
    - INVOKESTATIC android/util/Log.d (Ljava/lang/String;Ljava/lang/String;)I  
    可以看到ClickAgent.click(v)对应的字节码是两行； 
    - ALOAD 1 表示把局部变量表里索引为 1 的值，推到操作数栈上,也就是参数值 View v。
      - 对应到 ASM，是 methodVisitor.visitVarInsn(Opcodes.ALOAD, 1); 
    - INVOKESTATIC com/example/wangkai/ClickAgent.click (Landroid/view/View;)V就是调用 ClickAgent 的静态方法 click。
      - 对应到 ASM，是 methodVisitor.visitMethodInsn(Opcodes.INVOKESTATIC, "com/example/wangkai/ClickAgent", "click", "(Landroid/view/View;)V", false)
  - 当我们在 visitorMethod 回调里判断 name、desc 和 signature 和原始方法一致，并且该类实现的 interfaces 包含了 View$onClickListener 时，就可以注入了。 
  - 具体的注入代码如下：
    ```
    @Override
    void visitCode() {
    super.visitCode()
    mv.visitVarInsn(Opcodes.ALOAD, 1);
    mv.visitMethodInsn(Opcodes.INVOKESTATIC, "com/example/wangkai/ClickAgent", "click", "(Landroid/view/View;)V", false)
    }
    ```
    
  - 修改后执行，会生成插桩后的 class 文件，可以用 JD_GUI 查看插桩后的效果。 
    - 实际编码中，我们可以借助于 Bydecode Outline 插件。 
    
- 以上简单介绍了无埋点插桩实现的过程。
  - 实际的插件工程要复杂，需要考虑：
    - 黑白名单处理；
    - Manifest文件读取；
    - 插桩的统一处理等。
  - 另外，考虑到实现的复杂度和对性能的消耗，无埋点并不能完全代替手工埋点；
    - 部分埋点信息仍然需要手工补全。
