# 介绍
## 主要内容介绍
该项目为中国象棋棋牌室的实现，可以实现在线上对弈(使用本地回环地址,并没有在网上部署服务器)，借助Qt实现。
大概2700行C++代码。
其中主要包括4大部分：一是行棋规则的设计；二是服务端以及数据库的设计(这里使用sqlite3)；三是客户端的设计；四是客户端与服务端通讯协议和处理函数接口的设计
行棋规则主要包括，马走日字，象走田字，炮翻山等等。在该设计中，将所有兵种的行棋规则抽象为距离规则、范围规则、方向规则、前置规则、特殊规则。
服务端主要处理来自客户端的注册、登录、注销、与其它玩家对弈等需求，并根据客户端需求来操作和维护相应的数据库中的表。
客户端主要借助Qt实现界面。主要包括三个界面——>登录界面，棋牌室界面，下棋界面。其中登录界面要求首次登录的用户注册自己的账号；棋牌室界面用于查找在线玩家，选择对弈对手等功能。
客户端与服务端之间制定通讯协议，以及发送和接收协议的接口函数等。

## 展示
### 在Qt中启动程序

如下图所示，Qt Creator界面中总共打开了两个项目，一个为TCPClient（使用TCP/IP协议的客户端），一个为TcpServer（使用TCP/IP协议的服务端）。
![image](https://github.com/helloautomatic/personal-works/assets/88640443/5643ef00-31ce-4a29-b374-e912255e8fca)

### 先启动服务端
这里的服务端仅仅是个简单的窗口，为了表示服务端已经运行起来了。

![image](https://github.com/helloautomatic/personal-works/assets/88640443/c414a3c9-d8a4-4d5d-ae31-be0fff3d275d)

### 启动两个客户端

客户端可以启动多个，以模拟多个玩家在线

![image](https://github.com/helloautomatic/personal-works/assets/88640443/937f6296-5bc6-40f3-802c-dd2a3ba315d9)

登录选项，这里假设之前已经注册好了一个用户，用户名为player1，密码也为player1。同样的，注册好了另外一个用户player2，密码耶为player2。

![image](https://github.com/helloautomatic/personal-works/assets/88640443/78501f41-51f0-4422-b9d6-27fe1d74a893)

登录成功后，点击刷新列表，可以看到所有的在线玩家

![image](https://github.com/helloautomatic/personal-works/assets/88640443/5ba463cf-7ed2-4b5e-bf88-935e2e1b8bec)

### 对弈

两个用户对弈

![image](https://github.com/helloautomatic/personal-works/assets/88640443/30f65210-a6dd-42c0-9a03-f6c11b38f35d)

在选定对手后，就可以开始了。在游戏规则中，请求方为红方，即为先下棋的一方

![image](https://github.com/helloautomatic/personal-works/assets/88640443/e5dcb036-c544-4a2e-8d66-a9a912562401)

![image](https://github.com/helloautomatic/personal-works/assets/88640443/d438fb87-30bd-433f-8340-f3b84e76f327)

### 多玩家同时对弈

总共五个玩家在线，其中有两对玩家在对弈，也就是四个玩家在玩游戏，其余的一个玩家没找到匹配。当然了，这里没有考虑当玩家数量达到一定规模的时候，
服务端的负载问题。
![image](https://github.com/helloautomatic/personal-works/assets/88640443/e788969f-07d0-482b-a64f-e204824b99b4)



## 中国象棋新玩法的扩展及其介绍（该功能尚未实现）

上述仅仅是展示了象棋的传统的玩法。后期扩展该程序的思路主要有两个方面：

1.象棋新玩法，不同于传统象棋，新玩法中，可以实现棋子合体从而实现新的功能。比如说，将老将和马合体，那么老将就可以走出那个四方格。但是合体带来的弊端就是
，一死就是死两步棋。好处是，可以通过合体产生不同的功能。这加大了象棋玩法的难度，也增加了程序设计过程中的难度。

2.人机对战中的Robot的设计。

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

3.编译方式问题，如果qt采用的MSVC编译，代码文件是无BOM的utf-8编码格式,可以切换为GW编译，或者在pro文件里加入以下代码（加到.pro文件的末尾）:

```
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
```

**特别提醒：**
**修改完“.pro”文件或者修改完编译器选项后，最好选择重新分析构建整个项目，而不是在原来的基础上继续构建。**

## 运行问题

### 运行客户端可能遇到的问题
在运行代码的时候，如何在同一Qt Creator界面下打开多个客户端程序？

默认情况下（刚安装好的Qt Creator），一个Qt Creator界面只能运行一个程序，不过可以通过以下方式修改

**首先**

![image](https://github.com/helloautomatic/personal-works/assets/88640443/497fd688-47cb-4d64-8631-44fa88c85227)

**其次**

![image](https://github.com/helloautomatic/personal-works/assets/88640443/e61e9d0a-2e46-40a7-b2ae-da38f93c5f1e)

**运行多个客户端，用于模拟多个玩家在线的情况，比如运行三个客户端，代表三个玩家在线**
该作品并没有部署到互联网上，仅仅使用本低回环地址来模拟棋牌室。如果要部署到网上，代码的框架和逻辑是一样，可能只需要些许的调整，
并考虑网络通讯过程中可能需要处理的一些问题。

![image](https://github.com/helloautomatic/personal-works/assets/88640443/ee9f4d53-2795-4422-8fbf-86f108abbb30)

### 运行服务端可能遇到的问题
服务端的运行需要依赖数据库，这里的数据库。为了便于数据库的安装和部署，这里使用了一款轻量级的**关系型**数据库SQLite。**不过像SQL yog、Nvicat等工具可能并不支持SQLite**，所以在编写查询语句以及调试的时候会有点麻烦（只能在CMD中进行）。
**在启动客户端之前，需要先启动服务端。如果客户端和服务端的启动顺序颠倒了，会导致程序不能正常运行。**

### 数据库的安装
参考SQLite的官方安装教程。

### 数据库中表的创建








