/*
*	描述：错误号管理类
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

if (typeof error == "undefined") {
	var error = {};
	error.ERROR_CLIENT_SESSION 					= 10000001;				//session错误
	error.ERROR_SERVER_INNER 						= 10000002;				//服务器内部错误
	error.ERROR_CONFIG_NOT_EXIST 				= 10000003;				//配置文件错误
	error.ERROR_CONNECT_TIMEOUT 					= 10000004;				//连接超时
	error.ERROR_CLIENT_OPERATE 					= 10000005;				//非法操作
	error.ERROR_CLIENT_PARAM 						= 10000006;				//消息参数非法
	error.ERROR_DISCONNECT_RELOGIN				= 10000007;				//账号在其它地方登陆
	error.ERROR_DISCONNECT_IP 						= 10000008;				//ip已被封禁
	error.ERROR_DISCONNECT_ACCOUNT 			= 10000009;				//账号已被封禁
	error.ERROR_DISCONNECT_SELF 					= 10000010;				//服务重启中;稍候再试
	error.ERROR_DISCONNECT_ROLE 					= 10000011;				//角色被封
	error.ERROR_LOGIN_PLAYER_RECOVERING 	= 10000012;				//角色回收中;刷新再试
	error.ERROR_LOGIN_VERIFY_FAIL 				= 10000013;				//登录验证失败
	error.ERROR_VERIFY_MSG_SERIAL 				= 10000014;				//非法客户端
	error.ERROR_VERIFY_MSG_TIME 					= 10000015;				//非法客户端
	error.ERROR_LEVEL_LIMIT 						= 10000016;				//角色等级不足
	error.ERROR_ROLE_NOT_EXIST 					= 10000017;				//角色不存在
	error.ERROR_EXIST_ROLE_NAME 					= 10000018;				//角色名已存在
	error.ERROR_ROLE_OFFLINE 						= 10000019;				//玩家已离线
	error.ERROR_REGISTER_VERIFY_FAIL			= 10000020;				//注册验证失败
}
