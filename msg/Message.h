/*
 * Message.h
 *
 *  Created on: 2015年12月28日
 *      Author: zhangyalei
 */

#ifndef MSG_MESSAGE_H_
#define MSG_MESSAGE_H_

/*消息ID说明
110000-110999		Client--->Login
111000-111999		Client--->Gate
112000-112999		Login<--->Gate
113000-113999		Game<---->Gate
114000-114999		Master<--->Gate
115000-115999		Chat<--->Gate
120000-129999		Client--->Game
130000-139999		Client--->Master
140000-149999  	Client--->Chat
150000-159999  Game<--->DB
160000-169999		Game<--->Master
170000-179999		Master<--->DB
180000-184999		Game--->Log			程序日志
185000-189999		Game--->Stream		操作流水日志
190000-199999
300000-309999		ActiveMessage			服务器发到客户端主动消息
400000-499999		InnerMessage			服务器内部消息

510000-510999		Login->Client
511000-511999		Gate->Client
520000-529999		Game->Client
530000-539999		Master->Client
540000-549999		Chat->Client
550000-559999		DB-->Game
*/

enum {
	CLIENT_LOGIN_MESSAGE_START 				= 110000,
	CLIENT_LOGIN_MESSAGE_END 					= 110999,
	CLIENT_GATE_MESSAGE_START 					= 111000,
	CLIENT_GATE_MESSAGE_END 					= 111999,
	CLIENT_GAME_MESSAGE_START 					= 120000,
	CLIENT_GAME_MESSAGE_END 					= 129999,
};

enum Client_Login_Message {
	REQ_CLIENT_REGISTER 							= 	110000,  //注册请求
	REQ_CLIENT_LOGIN 									= 	110001,  //登录请求
};

enum Login_Client_MessageId{
	RES_CLIENT_REGISTER                = 510000,  //注册回复
	RES_CLIENT_LOGIN                 	= 510001,  //登录回复
};

enum Client_Gate_Message {
	REQ_HEARTBEAT 										=	111000,	//心跳
	REQ_CONNECT_GATE									=	111001,	//客户端登录连接gate
};

enum Gate_Client_Message{
	RES_HEARTBEAT 										= 	511000,	//心跳(返回)
	RES_CONNECT_GATE									= 511001,	//客户端登录连接gate(返回)
};

enum Client_Game_Message {
	REQ_FETCH_ROLE_INFO								=	120001,	//获取角色信息
	REQ_CREATE_ROLE 									= 	120002,	//创建角色

	REQ_FETCH_BAG_INFO 								= 120100,	//获取背包信息
	REQ_ADD_CAPACITY 									= 120101,	//增加容量
	REQ_DISCARD_ITEM 									= 120102,	//丢弃物品
	REQ_MOVE_ITEM 										= 120103,	//移动物品
	REQ_SPLIT_ITEM 										= 120104,	//拆分物品
	REQ_SORT_ITEM 										= 120105,	//整理物品
	REQ_MERGE_ITEM 										= 120106,	//合并物品
	REQ_SELL_ITEM 										= 120107,	//出售物品

	REQ_FETCH_MAIL_INFO 							= 120200,	//获取邮件信息
	REQ_PICKUP_MAIL 									= 120201,	//收取附件和金钱
	REQ_DEL_MAIL 											= 120202,	//删除邮件
	REQ_SEND_MAIL 										= 120203,	//发送邮件
};

enum Game_Client_Message {
	RES_FETCH_ROLE_INFO								=	520001,	//获取角色信息(返回)
	RES_CREATE_ROLE 									= 	520002,	//创建角色(返回)

	RES_FETCH_BAG_INFO 								= 520100,	//获取背包信息(返回)
	RES_ADD_CAPACITY 									= 520101,	//增加背包容量(返回)
	RES_DISCARD_ITEM 									= 520102,	//丢弃物品(返回)
	RES_MOVE_ITEM 										= 520103,	//移动物品(返回)
	RES_SPLIT_ITEM 										= 520104,	//拆分物品(返回)
	RES_SORT_ITEM 										= 520105,	//整理物品(返回)
	RES_MERGE_ITEM 										= 520106,	//合并物品(返回)
	RES_SELL_ITEM 										= 520107,	//出售物品(返回)

	RES_FETCH_MAIL_INFO								= 520200,	//获取邮件信息(返回)
	RES_PICKUP_MAIL 									=	520201,	//收取附件(返回)
	RES_DEL_MAIL 											= 520202,	//删除邮件(返回)
	RES_SEND_MAIL 										= 520203,	//发送邮件(返回)
};

//此次包含login,game,chat,master与gate通信的消息
enum Gate_Message {
	SYNC_GATE_LOGIN_PLAYER_ACCOUNT     = 112000,  //gate校验玩家账户、session
	SYNC_LOGIN_GATE_PLAYER_ACCOUNT     = 112001,  //login返回session校验结果
	SYNC_GATE_GAME_PLAYER_SIGNOUT 			= 	113000,	//gate同步玩家掉线到game
};

enum Game_DB_Message {
	SYNC_GAME_DB_LOAD_DB_CACHE					=	150000,	//加载db_cache
	SYNC_GAME_DB_LOAD_PLAYER_INFO			=	150001,	//加载玩家信息
	SYNC_GAME_DB_CREATE_PLAYER					=	150002,	//创建玩家
	SYNC_GAME_DB_SAVE_PLAYER_INFO			= 150003,	//保存玩家信息
};

enum DB_Game_Message {
	SYNC_DB_GAME_LOAD_DB_CACHE					=	550000,	//加载db_cache(返回)
	SYNC_DB_GAME_LOAD_PLAYER_INFO			=	550001,	//加载玩家信息(返回)
	SYNC_DB_GAME_CREATE_PLAYER					=	550002,	//创建玩家(返回)
	SYNC_DB_GAME_SAVE_PLAYER_INFO			=	550003,	//保存玩家信息(返回)
};

enum Game_Master_Message {
	SYNC_GAME_MASTER_PLYAER_SIGNIN			=	160001,	//同步玩家登录消息到master
	SYNC_GAME_MASTER_PLAYER_SIGNOUT		=	160002,	//同步玩家离线消息到master
};

enum Log_Message {
	SYNC_LOG_FILE_RECORD 							= 180000,	//记录日志文件
	SYNC_LOG_BUFFER_RECORD 						= 180001,	//记录buffer日志
};

enum Back_Message {
	SYNC_BACK_TEST_STREAM 						= 185000,	//后台MySQL数据库插入测试
	SYNC_BACK_LOGINOUT_STREAM 					= 185001,	//后台登录退出流水
};

enum Active_Message{
	ACTIVE_DISCONNECT 								=	300000,	//断开连接

	ACTIVE_ITEM_INFO 									= 300100,	//物品信息更新
	ACTIVE_ITEM_ADDIT_INFO 						= 300101,	//物品附加信息更新
	ACTIVE_BAG_CAPACITY 							= 300102,	//背包容量改变
	ACTIVE_MONEY_INFO 								= 300103,	//金钱更新
	ACTIVE_ITEM_ERASE 								= 300104,	//物品删除

	ACTIVE_RECEIVE_MAIL 							= 300200,	//收到邮件
};

enum Inner_Message{
	SYNC_INNER_TIMER_TICK 						= 	400000,	//定时器tick
	SYNC_INNER_CONFIG_HOTUPDATE 				= 	400001,	//配制文件热更消息
};

#endif /* MSG_MESSAGE_H_ */
