# 介绍

# 1 编译源码过程中可能遇到的问题
编译器选项中通常有GCC、MSVC、MinGW等。如果要将代码转移到别的计算机上运行，需要选择对应的编译器进行编译。

比如会遇到如下编译器报错：E:\QtProject\ChessV2\ChineseChess\TCPclient\moverules.cpp:93: error: C2001: 常量中有换行符

导致这种情况的原因可能是：

1.中文编码格式问题，在qt：工具-外部-配置里选择 文本编辑器-行为-默认编码选为UTF-8，UTF-8 BOM选为如果编码是UTF-8则添加。

2.依旧采用默认编码，在需要使用中文的地方使用QStringLiteral("text")来进行转码

3.编译方式问题，如果qt采用的MSVC编译，代码文件是无BOM的utf-8编码格式,可以切换为GW编译，或者在pro文件里加入以下代码:

```
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
```

