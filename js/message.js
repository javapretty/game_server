/*
*	描述：消息号管理类
*	作者：张亚磊
*	时间：2016/02/24
*	qq:784599938
*	tel:18268193219
*/

if (typeof msg_req == "undefined") {
	var msg_req = {};
	
	msg_req.SYNC_GATE_GAME_PLAYER_SIGNOUT 					= 	113000;	//gate同步玩家掉线到game
	
	msg_req.REQ_FETCH_ROLE_INFO								=	120001;	//获取角色信息
	msg_req.REQ_CREATE_ROLE 									= 	120002;	//创建角色

	msg_req.REQ_FETCH_BAG_INFO 								= 	120100;	//获取背包信息
	msg_req.REQ_USE_ITEM											= 120101;	//使用道具
	msg_req.REQ_SELL_ITEM 										= 	120102;	//出售道具

	msg_req.REQ_FETCH_MAIL_INFO 								= 	120200;	//获取邮件信息
	msg_req.REQ_PICKUP_MAIL 									= 	120201;	//收取附件和金钱
	msg_req.REQ_DEL_MAIL 											= 	120202;	//删除邮件
	msg_req.REQ_SEND_MAIL 										= 	120203;	//发送邮件
	
	msg_req.REQ_FETCH_HERO_INFO								=	120300;	//获取英雄信息
	msg_req.REQ_ADD_HERO_STAR									=	120301;	//提高英雄星级
	msg_req.REQ_ADD_HERO_QUALITY								= 120302;	//提高英雄品质
}

if (typeof msg_res == "undefined") {
	var msg_res = {};
	msg_res.RES_FETCH_ROLE_INFO								=	520001;	//获取角色信息(返回)
	msg_res.RES_CREATE_ROLE 									= 	520002;	//创建角色(返回)

	msg_res.RES_FETCH_BAG_INFO 								= 	520100;	//获取背包信息(返回)
	msg_res.RES_USE_ITEM 											= 	520101;	//使用道具(返回)
	msg_res.RES_SELL_ITEM 										= 	520102;	//出售道具(返回)

	msg_res.RES_FETCH_MAIL_INFO								= 	520200;	//获取邮件信息(返回)
	msg_res.RES_PICKUP_MAIL 									=	520201;	//收取附件(返回)
	msg_res.RES_DEL_MAIL 											= 	520202;	//删除邮件(返回)
	msg_res.RES_SEND_MAIL 										= 	520203;	//发送邮件(返回)
	
	msg_res.RES_FETCH_HERO_INFO								=	520300;	//获取英雄信息(返回)
	msg_res.RES_ADD_HERO_STAR									=	520301;	//提高英雄星级(返回)
	msg_res.RES_ADD_HERO_QUALITY								= 520302;	//提高英雄品质(返回)
}

if (typeof msg_active == "undefined") {
	var msg_active = {};
	msg_active.ACTIVE_DISCONNECT 							=	300000;	//断开连接

	msg_active.ACTIVE_ITEM_INFO 								= 300100;	//物品信息更新
	msg_active.ACTIVE_ITEM_ADDIT_INFO 					= 300101;	//物品附加信息更新
	msg_active.ACTIVE_MONEY_INFO 							= 300102;	//金钱更新

	msg_active.ACTIVE_RECEIVE_MAIL 						= 300200;	//收到邮件
}

if (typeof msg_db == "undefined") {
	var msg_db = {};
	msg_db.SYNC_GAME_DB_SAVE_MAIL_INFO					= 150004;	//保存邮件信息
}