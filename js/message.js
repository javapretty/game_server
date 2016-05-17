/** 
* This file was auto-generated. Please Do not edit
*
* [Version 1.3]
*/

if (typeof Msg_CL == "undefined") {
	var Msg_CL = {};
	Msg_CL.REQ_CLIENT_REGISTER = 100000; //注册
	Msg_CL.REQ_CLIENT_LOGIN = 100001; //登录
	Msg_CL.REQ_CONNECT_GATE = 100002; //客户端登录gate
	Msg_CL.REQ_HEARTBEAT = 100003; //心跳
}

if (typeof Msg_LC == "undefined") {
	var Msg_LC = {};
	Msg_LC.RES_CLIENT_REGISTER = 500000; //注册(返回)
	Msg_LC.RES_CLIENT_LOGIN = 500001; //登录(返回)
	Msg_LC.RES_CONNECT_GATE = 500002; //客户端登录gate(返回)
	Msg_LC.RES_HEARTBEAT = 500003; //心跳(返回)
}

if (typeof Msg_CM == "undefined") {
	var Msg_CM = {};
	Msg_CM.REQ_SEND_CHAT_INFO = 110001; //发送世界聊天消息
	Msg_CM.REQ_CREATE_GUILD = 110101; //申请创建公会
	Msg_CM.REQ_DISSOVE_GUILD = 110102; //申请解散公会
	Msg_CM.REQ_JOIN_GUILD = 110103; //申请加入公会
}

if (typeof Msg_MC == "undefined") {
	var Msg_MC = {};
	Msg_MC.RES_SEND_CHAT_INFO = 510001; //发送世界聊天消息(返回)
	Msg_MC.RES_CREATE_GUILD = 510101; //申请创建公会(返回)
	Msg_MC.RES_DISSOVE_GUILD = 510102; //申请解散公会(返回)
	Msg_MC.RES_JOIN_GUILD = 510103; //申请加入公会(返回)
}

if (typeof Msg_CG == "undefined") {
	var Msg_CG = {};
	Msg_CG.REQ_FETCH_ROLE_INFO = 120001; //获取角色信息
	Msg_CG.REQ_CREATE_ROLE = 120002; //创建角色
	Msg_CG.REQ_BUY_VITALITY = 120003; //购买体力
	Msg_CG.REQ_EXCHANGE_MONEY = 120004; //兑换聚宝盆
	Msg_CG.REQ_FETCH_BAG_INFO = 120100; //获取背包信息
	Msg_CG.REQ_USE_ITEM = 120101; //使用物品
	Msg_CG.REQ_SELL_ITEM = 120102; //出售物品
	Msg_CG.REQ_FETCH_MAIL_INFO = 120200; //获取邮件信息
	Msg_CG.REQ_PICKUP_MAIL = 120201; //收取附件和金钱
	Msg_CG.REQ_DEL_MAIL = 120202; //删除邮件
	Msg_CG.REQ_SEND_MAIL = 120203; //发送邮件
	Msg_CG.REQ_FETCH_HERO_INFO = 120300; //获取英雄信息
	Msg_CG.REQ_ADD_HERO_STAR = 120301; //提高英雄星级
	Msg_CG.REQ_ADD_HERO_QUALITY = 120302; //提高英雄品质
	Msg_CG.REQ_ADD_EQUIP_LEVEL = 120303; //提高英雄装备等级
	Msg_CG.REQ_EQUIP_ON_OFF = 120304; //英雄装备穿脱
	Msg_CG.REQ_ADD_SKILL_LEVEL = 120305; //提高英雄技能等级
	Msg_CG.REQ_FETCH_SHOP_INFO = 120400; //拉取商店信息
	Msg_CG.REQ_BUY_PRODUCT = 120401; //购买物品
	Msg_CG.REQ_REFRESH_SHOP = 120402; //刷新商店
}

if (typeof Msg_GC == "undefined") {
	var Msg_GC = {};
	Msg_GC.RES_FETCH_ROLE_INFO = 520001; //获取角色信息(返回)
	Msg_GC.RES_CREATE_ROLE = 520002; //创建角色(返回)
	Msg_GC.RES_BUY_VITALITY = 520003; //购买体力(返回)
	Msg_GC.RES_EXCHANGE_MONEY = 520004; //兑换聚宝盆(返回)
	Msg_GC.RES_FETCH_BAG_INFO = 520100; //获取背包信息(返回)
	Msg_GC.RES_USE_ITEM = 520101; //使用物品(返回)
	Msg_GC.RES_SELL_ITEM = 520102; //出售物品(返回)
	Msg_GC.RES_FETCH_MAIL_INFO = 520200; //获取邮件信息(返回)
	Msg_GC.RES_PICKUP_MAIL = 520201; //收取附件(返回)
	Msg_GC.RES_DEL_MAIL = 520202; //删除邮件(返回)
	Msg_GC.RES_SEND_MAIL = 520203; //发送邮件(返回)
	Msg_GC.RES_FETCH_HERO_INFO = 520300; //获取英雄信息(返回)
	Msg_GC.RES_ADD_HERO_STAR = 520301; //提高英雄星级(返回)
	Msg_GC.RES_ADD_HERO_QUALITY = 520302; //提高英雄品质(返回)
	Msg_GC.RES_ADD_EQUIP_LEVEL = 520303; //提高英雄装备等级(返回)
	Msg_GC.RES_EQUIP_ON_OFF = 520304; //英雄装备穿脱(返回)
	Msg_GC.RES_ADD_SKILL_LEVEL = 520305; //提高英雄技能等级(返回)
	Msg_GC.RES_FETCH_SHOP_INFO = 520400; //拉取商店信息(返回)
	Msg_GC.RES_BUY_PRODUCT = 520401; //购买物品(返回)
	Msg_GC.RES_REFRESH_SHOP = 520402; //刷新商店(返回)
}

if (typeof Msg_Db == "undefined") {
	var Msg_Db = {};
	Msg_Db.SYNC_GAME_DB_LOAD_DB_CACHE = 150000; //加载db_cache
	Msg_Db.SYNC_GAME_DB_LOAD_PLAYER_INFO = 150001; //加载玩家信息
	Msg_Db.SYNC_GAME_DB_CREATE_PLAYER = 150002; //创建玩家
	Msg_Db.SYNC_GAME_DB_SAVE_PLAYER_INFO = 150003; //保存玩家信息
	Msg_Db.SYNC_GAME_DB_SAVE_MAIL_INFO = 150004; //保存邮件信息
	Msg_Db.SYNC_DB_GAME_LOAD_DB_CACHE = 550000; //加载db_cache(返回)
	Msg_Db.SYNC_DB_GAME_LOAD_PLAYER_INFO = 550001; //加载玩家信息(返回)
	Msg_Db.SYNC_DB_GAME_CREATE_PLAYER = 550002; //创建玩家(返回)
	Msg_Db.SYNC_DB_GAME_SAVE_PLAYER_INFO = 550003; //保存玩家信息(返回)
}

if (typeof Msg_MD == "undefined") {
	var Msg_MD = {};
	Msg_MD.SYNC_MASTER_DB_LOAD_PUBLIC_INFO = 150101; //加载公共信息
	Msg_MD.SYNC_MASTER_DB_SAVE_GUILD_INFO = 150102; //保存公会信息
	Msg_MD.SYNC_MASTER_DB_DROP_GUILD_INFO = 150103; //删除公会信息
	Msg_MD.SYNC_DB_MASTER_LOAD_PUBLIC_INFO = 550101; //加载公共信息(返回)
	Msg_MD.SYNC_DB_MASTER_SAVE_GUILD_INFO = 550102; //保存公会信息(返回)
	Msg_MD.SYNC_DB_MASTER_DROP_GUILD_INFO = 550103; //删除公会信息(返回)
}

if (typeof Msg_GM == "undefined") {
	var Msg_GM = {};
	Msg_GM.SYNC_GAME_MASTER_PLYAER_SIGNIN = 160000; //game同步玩家上线到master
	Msg_GM.SYNC_GAME_MASTER_PLAYER_SIGNOUT = 160001; //game同步玩家下线到master
	Msg_GM.SYNC_MASTER_GAME_SET_GUILD = 160100; //master通知game设置公会信息
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
