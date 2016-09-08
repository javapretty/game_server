服务器是异步非阻塞的多进程多线程架构，运行于CentOS7操作系统。服务器底层使用C++编写，高效稳定，
使用TCP协议进行进程间以及与客户端的通信。同时也支持UDP、Websocket、Http协议。
网络通信协议使用自定义格式，具备加密功能，安全高效。游戏数据库支持MongoDB和Mysql，可以选择自己想要的数据库。
逻辑层使用简单高效的Javascript进行开发，集成Google V8引擎解析脚本，保证脚本运行效率，有js经验的开发人员可以快速上手。

服务器分为LoginServer，GateServer，GameServer，MasterServer，DBServer，LogServer。

LoginServer：客户端发送账号密码到LoginServer，LoginServer去AccountDb验证，
验证失败断开连接，验证成功，则生成session， 根据玩家账号hash和gate在线人数选择GateIp,GatePort，发送给客户端。

GateServer：接受客户端发过来的连接，向LoginServer进行session验证，验证成功玩家可以正常登录，
然后就开始接收客户端消息，转发到Game，Master，类似网关中转器。

GameServer：
1、创建角色，角色登录，从DbServer加载数据返回给client，存储玩家数据  
2、场景管理模块 
3、AOI模块
4、定时器管理模块 
5、怪物AI模块 
6、Player管理模块 
7、C++消息解析包装成js对象

MasterServer： 
1、管理GameServer，玩家在GameServer进程间跳转 
2、存储在线玩家数据，进行相关校验验证，存储每个玩家对应的gameserver 
3、存储公共数据，例如帮派，好友，邮件，聊天，组队

V8：V8引擎管理模块，js代码解析，执行js代码，C++函数导出 

DbServer：
1、建立mongodb引擎和存取数据接口 
2、建立Mysql引擎和存取数据接口

LogServer: 
1、实现写服务器文件日志功能 
2、实现写玩家操作日志到Mysql Log数据库
