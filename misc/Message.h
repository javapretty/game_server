/*
 * Message.h
 *
 *  Created on: Aug 2, 2016
 *      Author: zhangyalei
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

enum Message_Bound {
	CLIENT_GATE_MESSAGE_START 			= 100000,
	CLIENT_GATE_MESSAGE_END 				= 109999,
	CLIENT_MASTER_MESSAGE_START			= 110000,
	CLIENT_MASTER_MESSAGE_END 			= 119999,
	CLIENT_GAME_MESSAGE_START 			= 120000,
	CLIENT_GAME_MESSAGE_END 				= 129999,
};

enum Client_Login_Message {
	REQ_CONNECT_LOGIN 								= 100001, 	//客户端登录login
	REQ_CONNECT_GATE 								= 100101, 	//客户端登录gate
	REQ_SEND_HEARTBEAT	 							= 100102, 	//发送心跳到gate
};

enum Login_Client_Message {
	RES_CONNECT_LOGIN 								= 500001, 	//客户端登录login(返回)
	RES_CONNECT_GATE 								= 500101, 	//客户端登录gate(返回)
	RES_SEND_HEARTBEAT 							= 500102, 	//发送心跳到gate(返回)
};

enum Client_Game_Message {
	REQ_FETCH_ROLE_INFO							=	120001,	//获取角色信息
};

enum Game_Client_Message {
	RES_FETCH_ROLE_INFO							=	520001,	//获取角色信息(返回)
};

enum Gate_Message {
	SYNC_GATE_LOGIN_PLAYER_ACCOUNT 				= 140000, //gate向login校验玩家session
	SYNC_LOGIN_GATE_PLAYER_ACCOUNT 				= 140001, //login返回gate校验session结果
	SYNC_GATE_GAME_PLAYER_LOGOUT 					= 140100, //gate通知玩家下线到game
	SYNC_GATE_MASTER_PLAYER_LOGOUT 				= 140200, //gate通知玩家下线到master
	SYNC_GATE_MASTER_PLAYER_LOGIN 				= 140201, //gate通知玩家上线到master
	SYNC_MASTER_GATE_PLAYER_CHANGE_SCENE =	140202,	 //master通知gate玩家改变场景
};

enum Game_DB_Message {
	SYNC_GAME_DB_LOAD_PLAYER 				= 150001, //加载玩家信息
	SYNC_GAME_DB_CREATE_PLAYER 			= 150002, //创建玩家
	SYNC_GAME_DB_SAVE_PLAYER 				= 150003, //保存玩家信息
	SYNC_DB_GAME_LOAD_PLAYER 				= 550001, //加载玩家信息(返回)
	SYNC_DB_GAME_CREATE_PLAYER 			= 550002, //创建玩家(返回)
	SYNC_DB_GAME_SAVE_PLAYER 				= 550003, //保存玩家信息(返回)
};

enum Master_DB_Message {
	SYNC_MASTER_DB_CREATE_GUILD 		= 150100, //创建公会
	SYNC_MASTER_DB_LOAD_DATA 				= 150101, //加载数据库信息
	SYNC_MASTER_DB_DELETE_DATA 			= 150102, //删除数据库信息
	SYNC_MASTER_DB_SAVE_GUILD 			= 150103, //保存公会信息
	SYNC_MASTER_DB_SAVE_OFFLINE 		= 150104, //保存离线信息
	SYNC_MASTER_DB_SAVE_RANK 				= 150105, //保存排行榜信息
	SYNC_DB_MASTER_CREATE_GUILD 		= 550100, //创建公会(返回)
	SYNC_DB_MASTER_LOAD_GUILD 			= 550103, //加载公会信息
	SYNC_DB_MASTER_LOAD_OFFLINE 		= 550104, //加载离线信息
	SYNC_DB_MASTER_LOAD_RANK 				= 550105, //加载排行榜信息
};

enum Game_Master_Message {
	SYNC_GAME_MASTER_SCENE_INFO			= 160000, 	//game通知master场景信息
};

enum Active_Message {
	ACTIVE_DISCONNECT 								= 300000, //服务器主动断开连接
};

#endif /* MESSAGE_H_ */
