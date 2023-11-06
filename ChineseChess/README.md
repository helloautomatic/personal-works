# 介绍

# 复现该项目中可能遇到的问题总结
## 编译问题
编译器选项中通常有GCC、MSVC、MinGW等。如果要将代码转移到别的计算机上运行，而每个计算机上的Qt Creator可能不太一样（比如编译环境不一样），因此，需要选择对应的编译器进行编译。

比如会遇到如下编译器报错：
```
E:\QtProject\ChessV2\ChineseChess\TCPclient\moverules.cpp:93: error: C2001: 常量中有换行符
```

导致这种情况的原因可能是：

1.中文编码格式问题，在qt：工具-外部-配置里选择 文本编辑器-行为-默认编码选为UTF-8，UTF-8 BOM选为如果编码是UTF-8则添加。

2.依旧采用默认编码，在需要使用中文的地方使用QStringLiteral("text")来进行转码

3.编译方式问题，如果qt采用的MSVC编译，代码文件是无BOM的utf-8编码格式,可以切换为GW编译，或者在pro文件里加入以下代码（加到文件的末尾）:

```
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
```

**特别提醒：**
**修改完“.pro”文件或者修改完编译器选项后，最好选择重新分析构建整个项目，而不是在原来的基础上继续构建。**

## 运行问题
在运行代码的时候，如何在同一Qt Creator界面下打开多个客户端程序？

默认情况下（刚安装好的Qt Creator），一个Qt Creator界面只能运行一个程序，不过可以通过以下方式修改

首先
![image](https://github.com/helloautomatic/personal-works/assets/88640443/497fd688-47cb-4d64-8631-44fa88c85227)

其次
![image](https://github.com/helloautomatic/personal-works/assets/88640443/e61e9d0a-2e46-40a7-b2ae-da38f93c5f1e)





