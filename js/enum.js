/*
*	描述：枚举类型管理
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

if (typeof Data_Change == "undefined") {
	var Data_Change = {};
	Data_Change.PLAYER_CHANGE = 1;
	Data_Change.HERO_CHANGE = 2;
	Data_Change.BAG_CHANGE = 3;
	Data_Change.MAIL_CHANGE = 4;
}

if (typeof Msg_Db == "undefined") {
	var Msg_Db = {};
	Msg_Db.SYNC_GAME_DB_SAVE_MAIL_INFO					= 150004;	//保存邮件信息
}

if (typeof Error_Code == "undefined") {
	var Error_Code = {};
	
	Error_Code.ERROR_CLIENT_SESSION 					= 10000001;				//session错误
	Error_Code.ERROR_SERVER_INNER 						= 10000002;				//服务器内部错误
	Error_Code.ERROR_CONFIG_NOT_EXIST 				= 10000003;				//配置文件错误
	Error_Code.ERROR_CONNECT_TIMEOUT 					= 10000004;				//连接超时
	Error_Code.ERROR_CLIENT_OPERATE 					= 10000005;				//非法操作
	Error_Code.ERROR_CLIENT_PARAM 						= 10000006;				//消息参数非法
	Error_Code.ERROR_DISCONNECT_RELOGIN				= 10000007;				//账号在其它地方登陆
	Error_Code.ERROR_DISCONNECT_IP 						= 10000008;				//ip已被封禁
	Error_Code.ERROR_DISCONNECT_ACCOUNT 			= 10000009;				//账号已被封禁
	Error_Code.ERROR_DISCONNECT_SELF 					= 10000010;				//服务重启中;稍候再试
	Error_Code.ERROR_DISCONNECT_ROLE 					= 10000011;				//角色被封
	Error_Code.ERROR_LOGIN_PLAYER_RECOVERING 	= 10000012;				//角色回收中;刷新再试
	Error_Code.ERROR_VERIFY_MSG_SERIAL 				= 10000014;				//非法客户端
	Error_Code.ERROR_VERIFY_MSG_TIME 					= 10000015;				//非法客户端
	Error_Code.ERROR_LEVEL_LIMIT 						= 10000016;				//角色等级不足
	Error_Code.ERROR_ROLE_NOT_EXIST 					= 10000017;				//角色不存在
	Error_Code.ERROR_EXIST_ROLE_NAME 					= 10000018;				//角色名已存在
	Error_Code.ERROR_ROLE_OFFLINE 						= 10000019;				//玩家已离线
	Error_Code.ERROR_REGISTER_VERIFY_FAIL			= 10000020;				//注册验证失败
	Error_Code.ERROR_BAG_FULL								= 10000100,				//背包已满
	Error_Code.ERROR_ITEM_NOT_EXIST						= 10000101;				//物品不存在
	Error_Code.ERROR_ITEM_NOT_ENOUGH					= 10000102;				//物品不足
	Error_Code.ERROR_COPPER_NOT_ENOUGH				= 10000103;				//铜钱不足
	Error_Code.ERROR_GOLD_NOT_ENOUGH					= 10000104;				//元宝不足
}

if (typeof Msg_Req == "undefined") {
	var Msg_Req = {};
	
	Msg_Req.SYNC_GATE_GAME_PLAYER_SIGNOUT 			= 	140002;	//gate同步玩家掉线到game
	
	Msg_Req.REQ_FETCH_ROLE_INFO								=	120001;	//获取角色信息
	Msg_Req.REQ_CREATE_ROLE 									= 	120002;	//创建角色
	Msg_Req.REQ_BUY_VITALITY									= 120003;	//提高玩家体力
	
	Msg_Req.REQ_FETCH_BAG_INFO 								= 	120100;	//获取背包信息
	Msg_Req.REQ_USE_ITEM											= 120101;	//使用道具
	Msg_Req.REQ_SELL_ITEM 										= 	120102;	//出售道具

	Msg_Req.REQ_FETCH_MAIL_INFO 								= 	120200;	//获取邮件信息
	Msg_Req.REQ_PICKUP_MAIL 									= 	120201;	//收取附件和金钱
	Msg_Req.REQ_DEL_MAIL 											= 	120202;	//删除邮件
	Msg_Req.REQ_SEND_MAIL 										= 	120203;	//发送邮件
	
	Msg_Req.REQ_FETCH_HERO_INFO								=	120300;	//获取英雄信息
	Msg_Req.REQ_ADD_HERO_STAR									=	120301;	//提高英雄星级
	Msg_Req.REQ_ADD_HERO_QUALITY								= 120302;	//提高英雄品质
	Msg_Req.REQ_ADD_EQUIP_LEVEL								= 120303;	//提高英雄装备等级
	Msg_Req.REQ_EQUIP_ON_OFF									=	120304;	//英雄装备穿脱
}

if (typeof Msg_Res == "undefined") {
	var Msg_Res = {};
	Msg_Res.RES_FETCH_ROLE_INFO								=	520001;	//获取角色信息(返回)
	Msg_Res.RES_CREATE_ROLE 									= 	520002;	//创建角色(返回)
	Msg_Res.RES_BUY_VITALITY               			= 520003; //玩家购买体力(返回)
	
	Msg_Res.RES_FETCH_BAG_INFO 								= 	520100;	//获取背包信息(返回)
	Msg_Res.RES_USE_ITEM 											= 	520101;	//使用道具(返回)
	Msg_Res.RES_SELL_ITEM 										= 	520102;	//出售道具(返回)

	Msg_Res.RES_FETCH_MAIL_INFO								= 	520200;	//获取邮件信息(返回)
	Msg_Res.RES_PICKUP_MAIL 									=	520201;	//收取附件(返回)
	Msg_Res.RES_DEL_MAIL 											= 	520202;	//删除邮件(返回)
	Msg_Res.RES_SEND_MAIL 										= 	520203;	//发送邮件(返回)
	
	Msg_Res.RES_FETCH_HERO_INFO								=	520300;	//获取英雄信息(返回)
	Msg_Res.RES_ADD_HERO_STAR									=	520301;	//提高英雄星级(返回)
	Msg_Res.RES_ADD_HERO_QUALITY								= 520302;	//提高英雄品质(返回)
	Msg_Res.RES_ADD_EQUIP_LEVEL								= 520303;	//提高英雄装备等级(返回)
	Msg_Res.RES_EQUIP_ON_OFF									=	520304;	//英雄装备穿脱(返回)
}

if (typeof Msg_Active == "undefined") {
	var Msg_Active = {};
	
	Msg_Active.ACTIVE_DISCONNECT 							=	300000;	//断开连接

	Msg_Active.ACTIVE_ITEM_INFO 								= 300100;	//物品信息更新
	Msg_Active.ACTIVE_ITEM_ADDIT_INFO 					= 300101;	//物品附加信息更新
	Msg_Active.ACTIVE_MONEY_INFO 							= 300102;	//金钱更新

	Msg_Active.ACTIVE_RECEIVE_MAIL 						= 300200;	//收到邮件
}
