/*
*	描述：消息号管理类
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

if (typeof MSG_REQ == "undefined") {
	var MSG_REQ = {};
	
	MSG_REQ.SYNC_GATE_GAME_PLAYER_SIGNOUT 			= 	140002;	//gate同步玩家掉线到game
	
	MSG_REQ.REQ_FETCH_ROLE_INFO								=	120001;	//获取角色信息
	MSG_REQ.REQ_CREATE_ROLE 									= 	120002;	//创建角色

	MSG_REQ.REQ_FETCH_BAG_INFO 								= 	120100;	//获取背包信息
	MSG_REQ.REQ_USE_ITEM											= 120101;	//使用道具
	MSG_REQ.REQ_SELL_ITEM 										= 	120102;	//出售道具

	MSG_REQ.REQ_FETCH_MAIL_INFO 								= 	120200;	//获取邮件信息
	MSG_REQ.REQ_PICKUP_MAIL 									= 	120201;	//收取附件和金钱
	MSG_REQ.REQ_DEL_MAIL 											= 	120202;	//删除邮件
	MSG_REQ.REQ_SEND_MAIL 										= 	120203;	//发送邮件
	
	MSG_REQ.REQ_FETCH_HERO_INFO								=	120300;	//获取英雄信息
	MSG_REQ.REQ_ADD_HERO_STAR									=	120301;	//提高英雄星级
	MSG_REQ.REQ_ADD_HERO_QUALITY								= 120302;	//提高英雄品质
}

if (typeof MSG_RES == "undefined") {
	var MSG_RES = {};
	MSG_RES.RES_FETCH_ROLE_INFO								=	520001;	//获取角色信息(返回)
	MSG_RES.RES_CREATE_ROLE 									= 	520002;	//创建角色(返回)

	MSG_RES.RES_FETCH_BAG_INFO 								= 	520100;	//获取背包信息(返回)
	MSG_RES.RES_USE_ITEM 											= 	520101;	//使用道具(返回)
	MSG_RES.RES_SELL_ITEM 										= 	520102;	//出售道具(返回)

	MSG_RES.RES_FETCH_MAIL_INFO								= 	520200;	//获取邮件信息(返回)
	MSG_RES.RES_PICKUP_MAIL 									=	520201;	//收取附件(返回)
	MSG_RES.RES_DEL_MAIL 											= 	520202;	//删除邮件(返回)
	MSG_RES.RES_SEND_MAIL 										= 	520203;	//发送邮件(返回)
	
	MSG_RES.RES_FETCH_HERO_INFO								=	520300;	//获取英雄信息(返回)
	MSG_RES.RES_ADD_HERO_STAR									=	520301;	//提高英雄星级(返回)
	MSG_RES.RES_ADD_HERO_QUALITY								= 520302;	//提高英雄品质(返回)
}

if (typeof MSG_ACTIVE == "undefined") {
	var MSG_ACTIVE = {};
	
	MSG_ACTIVE.ACTIVE_DISCONNECT 							=	300000;	//断开连接

	MSG_ACTIVE.ACTIVE_ITEM_INFO 								= 300100;	//物品信息更新
	MSG_ACTIVE.ACTIVE_ITEM_ADDIT_INFO 					= 300101;	//物品附加信息更新
	MSG_ACTIVE.ACTIVE_MONEY_INFO 							= 300102;	//金钱更新

	MSG_ACTIVE.ACTIVE_RECEIVE_MAIL 						= 300200;	//收到邮件
}

if (typeof MSG_DB == "undefined") {
	var MSG_DB = {};
	MSG_DB.SYNC_GAME_DB_SAVE_MAIL_INFO					= 150004;	//保存邮件信息
}