/*
 * Error_Code.h
 *
 *  Created on: 2015年12月23日
 *      Author: zhangyalei
 */

#ifndef _ERRCODE_H_
#define _ERRCODE_H_

enum Error_Code {
	ERROR_SERVER_INNER 							= 10000,				//服务器内部错误
	ERROR_CLIENT_SESSION 						= 10001,				//session错误
	ERROR_CONNECT_TIMEOUT 					= 10002,				//连接超时
	ERROR_MSG_COUNT									= 10003,				//消息数量过多
	ERROR_MSG_SERIAL 								= 10004,				//消息序列号错误
	ERROR_MSG_TIME 									= 10005,				//消息时间错误
	ERROR_REGISTER_VERIFY_FAIL				= 10006,				//注册验证失败
	ERROR_LOGIN_VERIFY_FAIL					= 10007,				//登录验证失败
	ERROR_DISCONNECT_RELOGIN				= 10008,				//账号在其它地方登陆
	ERROR_DISCONNECT_RECOVERING 			= 10009,				//角色回收中,刷新再试
	ERROR_DISCONNECT_ACCOUNT 				= 10010,				//账号已被封禁
	ERROR_DISCONNECT_SELF 					= 10011,				//服务重启中,稍候再试
	ERROR_DISCONNECT_MASTER 				= 10012,				//玩家未登陆到master服务器

	ERROR_CLIENT_OPERATE 						= 10050,				//客户端非法操作
	ERROR_CLIENT_PARAM 							= 10051,				//客户端参数错误
	ERROR_CONFIG_NOT_EXIST 					= 10052,				//配置文件错误

	ERROR_ROLE_NOT_EXIST 						= 10100,				//角色不存在
	ERROR_ROLE_NAME_EXIST 					= 10101,				//角色名已存在
	ERROR_ROLE_OFFLINE 							= 10102,				//玩家已离线
};

#endif /* _ERRCODE_H_ */
