V1.0版本中存在许多问题，和漏洞还没未能及时搞定。主要问题如下：

1. C++ 源码在和SQlite3数据库进行交互的时候，会出现一些问题
2. TCP/IP通信这块，会存在一些问题：服务端**有时候**在释放客户端socket连接时，会导致内存访问冲突