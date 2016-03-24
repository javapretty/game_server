/** 
* struct JS file description
* 
* This file was auto-generated. Please Do not edit
* 
* [Version 1.1]
*
*/

if (typeof Msg_Req == "undefined") {
	var Msg_Req = {};
	Msg_Req.REQ_FETCH_ROLE_INFO = 120001; //获取角色信息
	Msg_Req.REQ_CREATE_ROLE = 120002; //创建角色
	Msg_Req.REQ_BUY_VITALITY = 120003; //购买体力
	Msg_Req.REQ_FETCH_BAG_INFO = 120100; //获取背包信息
	Msg_Req.REQ_USE_ITEM = 120101; //使用物品
	Msg_Req.REQ_SELL_ITEM = 120102; //出售物品
	Msg_Req.REQ_FETCH_MAIL_INFO = 120200; //获取邮件信息
	Msg_Req.REQ_PICKUP_MAIL = 120201; //收取附件和金钱
	Msg_Req.REQ_DEL_MAIL = 120202; //删除邮件
	Msg_Req.REQ_SEND_MAIL = 120203; //发送邮件
	Msg_Req.REQ_FETCH_HERO_INFO = 120300; //获取英雄信息
	Msg_Req.REQ_ADD_HERO_STAR = 120301; //提高英雄星级
	Msg_Req.REQ_ADD_HERO_QUALITY = 120302; //提高英雄品质
	Msg_Req.REQ_ADD_EQUIP_LEVEL = 120303; //提高英雄装备等级
	Msg_Req.REQ_EQUIP_ON_OFF = 120304; //英雄装备穿脱
	Msg_Req.REQ_ADD_SKILL_LEVEL = 120305; //提高英雄技能等级
	Msg_Req.REQ_FETCH_SHOP_INFO = 120400; //拉取商店信息
	Msg_Req.REQ_BUY_PRODUCT = 120401; //购买物品
	Msg_Req.REQ_REFRESH_SHOP = 120402; //刷新商店
}

if (typeof Msg_Res == "undefined") {
	var Msg_Res = {};
	Msg_Res.RES_FETCH_ROLE_INFO = 520001; //获取角色信息(返回)
	Msg_Res.RES_CREATE_ROLE = 520002; //创建角色(返回)
	Msg_Res.RES_BUY_VITALITY = 520003; //购买体力(返回)
	Msg_Res.RES_FETCH_BAG_INFO = 520100; //获取背包信息(返回)
	Msg_Res.RES_USE_ITEM = 520101; //使用物品(返回)
	Msg_Res.RES_SELL_ITEM = 520102; //出售物品(返回)
	Msg_Res.RES_FETCH_MAIL_INFO = 520200; //获取邮件信息(返回)
	Msg_Res.RES_PICKUP_MAIL = 520201; //收取附件(返回)
	Msg_Res.RES_DEL_MAIL = 520202; //删除邮件(返回)
	Msg_Res.RES_SEND_MAIL = 520203; //发送邮件(返回)
	Msg_Res.RES_FETCH_HERO_INFO = 520300; //获取英雄信息(返回)
	Msg_Res.RES_ADD_HERO_STAR = 520301; //提高英雄星级(返回)
	Msg_Res.RES_ADD_HERO_QUALITY = 520302; //提高英雄品质(返回)
	Msg_Res.RES_ADD_EQUIP_LEVEL = 520303; //提高英雄装备等级(返回)
	Msg_Res.RES_EQUIP_ON_OFF = 520304; //英雄装备穿脱(返回)
	Msg_Res.RES_ADD_SKILL_LEVEL = 520305; //提高英雄技能等级(返回)
	Msg_Res.RES_FETCH_SHOP_INFO = 520400; //拉取商店信息(返回)
	Msg_Res.RES_BUY_PRODUCT = 520401; //购买物品(返回)
	Msg_Res.RES_REFRESH_SHOP = 520402; //刷新商店(返回)
}

if (typeof Msg_Db == "undefined") {
	var Msg_Db = {};
	Msg_Db.SYNC_GAME_DB_LOAD_DB_CACHE = 150000; //加载db_cache
	Msg_Db.SYNC_GAME_DB_LOAD_PLAYER_INFO = 150001; //加载玩家信息
	Msg_Db.SYNC_GAME_DB_CREATE_PLAYER = 150002; //创建玩家
	Msg_Db.SYNC_GAME_DB_SAVE_PLAYER_INFO = 150003; //保存玩家信息
	Msg_Db.SYNC_GAME_DB_SAVE_MAIL_INFO = 150004; //保存邮件信息
}

if (typeof Msg_Active == "undefined") {
	var Msg_Active = {};
	Msg_Active.ACTIVE_DISCONNECT = 300000; //服务器主动断开连接
	Msg_Active.ACTIVE_PLAYER_INFO = 300001; //更新玩家信息
	Msg_Active.ACTIVE_VIP_INFO = 300002; //更新vip信息
	Msg_Active.ACTIVE_MONEY_INFO = 300100; //金钱更新
	Msg_Active.ACTIVE_ITEM_INFO = 300101; //物品信息更新
	Msg_Active.ACTIVE_MAIL_INFO = 300200; //邮件信息更新
	Msg_Active.ACTIVE_PROPERTY_INFO = 300300; //英雄属性信息更新
}
