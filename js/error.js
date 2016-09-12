/*
*	描述：错误号管理
*	作者：张亚磊
*	时间：2016/03/14
*/

if (typeof Error_Code == "undefined") {
	var Error_Code = {};

	Error_Code.ERROR_SERVER_INNER 							= 10000;				//服务器内部错误
	Error_Code.ERROR_CLIENT_SESSION 						= 10001;				//session错误
	Error_Code.ERROR_CONNECT_TIMEOUT 					= 10002;				//连接超时
	Error_Code.ERROR_MSG_COUNT									= 10003;				//消息数量过多
	Error_Code.ERROR_MSG_SERIAL 								= 10004;				//消息序列号错误
	Error_Code.ERROR_MSG_TIME 									= 10005;				//消息时间错误
	Error_Code.ERROR_DISCONNECT_SELF 					= 10006;				//服务重启中,稍候再试
	Error_Code.ERROR_DISCONNECT_RELOGIN				= 10007;				//账号在其它地方登陆
	Error_Code.ERROR_LOGIN_FAIL								= 10008;				//登录失败
	Error_Code.ERROR_NOT_LOGIN									=	10009;				//玩家未登录
	Error_Code.ERROR_SESSION_TIMEOUT						=	10010;				//session到期
	Error_Code.ERROR_SESSION_SUCCESS						=	10011;				//login验证session成功，断开连接

	Error_Code.ERROR_CLIENT_OPERATE 						= 10050;				//客户端非法操作
	Error_Code.ERROR_CLIENT_PARAM 							= 10051;				//客户端参数错误
	Error_Code.ERROR_CONFIG_NOT_EXIST 					= 10052;				//配置文件错误
	
	Error_Code.ERROR_ROLE_NOT_EXIST 						= 10100;				//角色名已存在
	Error_Code.ERROR_ROLE_NAME_EXIST 					= 10101;				//角色名已存在
	Error_Code.ERROR_ROLE_OFFLINE 							= 10102;				//玩家已离线
	Error_Code.ERROR_LEVEL_NOT_ENOUGH 					= 10103;				//角色等级不足
	Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH	=	10104;				//购买体力次数不足
	Error_Code.ERROR_SKILL_POINT_NOT_ENOUGH			=	10105;				//技能点不足
	
	Error_Code.ERROR_BAG_FULL									= 10200,				//背包已满
	Error_Code.ERROR_ITEM_NOT_EXIST						= 10201;				//物品不存在
	Error_Code.ERROR_ITEM_NOT_ENOUGH						= 10202;				//物品不足
	Error_Code.ERROR_COPPER_NOT_ENOUGH					= 10203;				//铜钱不足
	Error_Code.ERROR_GOLD_NOT_ENOUGH						= 10204;				//元宝不足
	Error_Code.ERROR_PRODUCT_NOT_EXIST					= 10205;				//商品不存在
	Error_Code.ERROR_PRODUCT_NOT_ENOUGH				= 10206;				//商品数量不足
	Error_Code.ERROR_REFRESH_NOT_ENOUGH				= 10207;				//商店刷新次数耗尽
	Error_Code.ERROR_EXCHANGE_COUNT_NOT_ENOUGH	= 10208;				//兑换元宝次数耗尽
	
	Error_Code.ERROR_GUILD_EXIST								= 10300;				//公会已存在
	Error_Code.ERROR_GUILD_NOT_EXIST						= 10301;				//公会不存在
}
