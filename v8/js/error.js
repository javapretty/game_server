if (typeof ErrorCode == "undefined") {
	var ErrorCode = {};
	ErrorCode.ERROR_CLIENT_SESSION 					= 10000001,				//session错误
	ErrorCode.ERROR_SERVER_INNER 						= 10000002,				//服务器内部错误
	ErrorCode.ERROR_CONFIG_NOT_EXIST 				= 10000003,				//配置文件错误
	ErrorCode.ERROR_CONNECT_TIMEOUT 					= 10000004,				//连接超时
	ErrorCode.ERROR_CLIENT_OPERATE 					= 10000005,				//非法操作
	ErrorCode.ERROR_CLIENT_PARAM 						= 10000006,				//消息参数非法
	ErrorCode.ERROR_DISCONNECT_RELOGIN				= 10000007,				//账号在其它地方登陆
	ErrorCode.ERROR_DISCONNECT_IP 					= 10000008,				//ip已被封禁
	ErrorCode.ERROR_DISCONNECT_ACCOUNT 				= 10000009,				//账号已被封禁
	ErrorCode.ERROR_DISCONNECT_SELF 					= 10000010,				//服务重启中,稍候再试
	ErrorCode.ERROR_DISCONNECT_ROLE 					= 10000011,				//角色被封
	ErrorCode.ERROR_LOGIN_PLAYER_RECOVERING 		= 10000012,				//角色回收中,刷新再试
	ErrorCode.ERROR_LOGIN_VERIFY_FAIL 				= 10000013,				//登录验证失败
	ErrorCode.ERROR_VERIFY_MSG_SERIAL 				= 10000014,				//非法客户端
	ErrorCode.ERROR_VERIFY_MSG_TIME 					= 10000015,				//非法客户端
	ErrorCode.ERROR_LEVEL_LIMIT 						= 10000016,				//角色等级不足
	ErrorCode.ERROR_ROLE_NOT_EXIST 					= 10000017,				//角色不存在
	ErrorCode.ERROR_EXIST_ROLE_NAME 					= 10000018,				//角色名已存在
	ErrorCode.ERROR_ROLE_OFFLINE 						= 10000019,				//玩家已离线
	ErrorCode.ERROR_REGISTER_VERIFY_FAIL			= 10000020,				//注册验证失败
}
