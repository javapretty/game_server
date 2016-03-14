/*
*	描述：错误号管理
*	作者：张亚磊
*	时间：2016/03/14
*	qq:784599938
*	tel:18268193219
*/

if (typeof Error_Code == "undefined") {
	var Error_Code = {};
	
	Error_Code.ERROR_SERVER_INNER 							= 10000;				//服务器内部错误
	Error_Code.ERROR_CLIENT_SESSION 						= 10001;				//session错误
	Error_Code.ERROR_CONNECT_TIMEOUT 					= 10002;				//连接超时
	Error_Code.ERROR_VERIFY_MSG_SERIAL 				= 10003;				//包序列号错误
	Error_Code.ERROR_VERIFY_MSG_TIME 					= 10004;				//包时间错误
	Error_Code.ERROR_DISCONNECT_RELOGIN				= 10005;				//账号在其它地方登陆
	Error_Code.ERROR_DISCONNECT_RECOVERING 			= 10006;				//角色回收中;刷新再试
	Error_Code.ERROR_DISCONNECT_ACCOUNT 				= 10007;				//账号已被封禁
	Error_Code.ERROR_DISCONNECT_SELF 					= 10008;				//服务重启中;稍候再试
	Error_Code.ERROR_CLIENT_OPERATE 						= 10009;				//非法操作
	Error_Code.ERROR_CLIENT_PARAM 							= 10010;				//消息参数非法
	Error_Code.ERROR_CONFIG_NOT_EXIST 					= 10011;				//配置文件错误
	Error_Code.ERROR_REGISTER_VERIFY_FAIL				= 10012;				//注册验证失败
	Error_Code.ERROR_LOGIN_VERIFY_FAIL					= 10013;				//登录验证失败
	
	Error_Code.ERROR_LEVEL_NOT_ENOUGH 					= 10100;				//角色等级不足
	Error_Code.ERROR_ROLE_NOT_EXIST 						= 10101;				//角色不存在
	Error_Code.ERROR_ROLE_NAME_EXIST 					= 10102;				//角色名已存在
	Error_Code.ERROR_ROLE_OFFLINE 							= 10103;				//玩家已离线
	Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH	=	10104;				//购买体力次数不足
	
	Error_Code.ERROR_BAG_FULL									= 10200,				//背包已满
	Error_Code.ERROR_ITEM_NOT_EXIST						= 10201;				//物品不存在
	Error_Code.ERROR_ITEM_NOT_ENOUGH						= 10202;				//物品不足
	Error_Code.ERROR_COPPER_NOT_ENOUGH					= 10203;				//铜钱不足
	Error_Code.ERROR_GOLD_NOT_ENOUGH						= 10204;				//元宝不足
}