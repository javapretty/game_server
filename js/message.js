
if (typeof Msg_CM == "undefined") {
	var Msg_CM = {};
	Msg_CM.REQ_SEND_CHAT_INFO = 110001; //发送世界聊天消息
	Msg_CM.REQ_CREATE_GUILD = 110101; //申请创建公会
	Msg_CM.REQ_DISSOVE_GUILD = 110102; //申请解散公会
	Msg_CM.REQ_JOIN_GUILD = 110103; //申请加入公会
	Msg_CM.REQ_GUILD_ALLOW_JOIN = 110104; //允许入帮
	Msg_CM.REQ_GUILD_KICK_OUT = 110105; //踢出帮会
	Msg_CM.REQ_FETCH_RANK = 110201; //拉取排行榜信息
}

if (typeof Msg_MC == "undefined") {
	var Msg_MC = {};
	Msg_MC.RES_SEND_CHAT_INFO = 510001; //发送世界聊天消息(返回)
	Msg_MC.RES_CREATE_GUILD = 510101; //申请创建公会(返回)
	Msg_MC.RES_DISSOVE_GUILD = 510102; //申请解散公会(返回)
	Msg_MC.RES_JOIN_GUILD = 510103; //申请加入公会(返回)
	Msg_MC.RES_GUILD_ALLOW_JOIN = 510104; //允许入帮(返回)
	Msg_MC.RES_GUILD_KICK_OUT = 510105; //踢出帮会(返回)
	Msg_MC.RES_FETCH_RANK = 510201; //拉取排行榜信息(返回)
	Msg_MC.RES_PLAYER_CHANGE_SCENE = 510300; //切换场景时通知客户端选择哪个game
}

if (typeof Msg_CG == "undefined") {
	var Msg_CG = {};
	Msg_CG.REQ_FETCH_ROLE_INFO = 120001; //获取角色信息
	Msg_CG.REQ_CREATE_ROLE = 120002; //创建角色
	Msg_CG.REQ_BUY_VITALITY = 120003; //购买体力
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
	Msg_CG.REQ_MOVE_TO_POINT = 120400; //移动到某点
	Msg_CG.REQ_CHANGE_SCENE = 120401; //切换场景
}

if (typeof Msg_GC == "undefined") {
	var Msg_GC = {};
	Msg_GC.RES_FETCH_ROLE_INFO = 520001; //获取角色信息(返回)
	Msg_GC.RES_CREATE_ROLE = 520002; //创建角色(返回)
	Msg_GC.RES_BUY_VITALITY = 520003; //购买体力(返回)
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
	Msg_GC.RES_SYNC_AOI_INFO = 520400; //通知场景中AOI信息(返回)
	Msg_GC.RES_CHANGE_SCENE = 520401; //切换场景(返回)
}

if (typeof Msg_Gate == "undefined") {
	var Msg_Gate = {};
	Msg_Gate.SYNC_GATE_LOGIN_PLAYER_ACCOUNT = 140000; //gate校验玩家账户、session
	Msg_Gate.SYNC_LOGIN_GATE_PLAYER_ACCOUNT = 140001; //login返回session校验结果
	Msg_Gate.SYNC_GATE_GAME_PLAYER_LOGOUT = 140100; //gate同步玩家下线到game
	Msg_Gate.SYNC_GATE_MASTER_PLAYER_LOGOUT = 140200; //gate同步玩家下线到master
	Msg_Gate.SYNC_GATE_MASTER_PLAYER_LOGIN = 140201; //gate同步玩家上线到master
}

if (typeof Msg_GD == "undefined") {
	var Msg_GD = {};
	Msg_GD.SYNC_GAME_DB_LOAD_PLAYER = 150001; //加载玩家信息
	Msg_GD.SYNC_GAME_DB_CREATE_PLAYER = 150002; //创建玩家
	Msg_GD.SYNC_GAME_DB_SAVE_PLAYER = 150003; //保存玩家信息
	Msg_GD.SYNC_DB_GAME_LOAD_PLAYER = 550001; //加载玩家信息(返回)
	Msg_GD.SYNC_DB_GAME_CREATE_PLAYER = 550002; //创建玩家(返回)
	Msg_GD.SYNC_DB_GAME_SAVE_PLAYER = 550003; //保存玩家信息(返回)
}

if (typeof Msg_MD == "undefined") {
	var Msg_MD = {};
	Msg_MD.SYNC_MASTER_DB_CREATE_GUILD = 150100; //创建公会
	Msg_MD.SYNC_MASTER_DB_LOAD_DATA = 150101; //加载数据库信息
	Msg_MD.SYNC_MASTER_DB_DELETE_DATA = 150102; //删除数据库信息
	Msg_MD.SYNC_MASTER_DB_SAVE_GUILD = 150103; //保存公会信息
	Msg_MD.SYNC_MASTER_DB_SAVE_OFFLINE = 150104; //保存离线信息
	Msg_MD.SYNC_MASTER_DB_SAVE_RANK = 150105; //保存排行榜信息
	Msg_MD.SYNC_DB_MASTER_CREATE_GUILD = 550100; //创建公会(返回)
	Msg_MD.SYNC_DB_MASTER_LOAD_GUILD = 550103; //加载公会信息
	Msg_MD.SYNC_DB_MASTER_LOAD_OFFLINE = 550104; //加载离线信息
	Msg_MD.SYNC_DB_MASTER_LOAD_RANK = 550105; //加载排行榜信息
}

if (typeof Msg_GM == "undefined") {
	var Msg_GM = {};
	Msg_GM.SYNC_GAME_MASTER_PLYAER_LOGIN = 160000; //game同步玩家上线到master
	Msg_GM.SYNC_MASTER_GAME_GUILD_INFO = 160100; //master通知game公会信息
	Msg_GM.SYNC_PLAYER_CHANGE_SCENE = 160101; //game通知master玩家请求切换其他game上的场景
	Msg_GM.SYNC_GAME_SCENE_ID = 160102; //通知master每个game上的场景信息
}

if (typeof Msg_Log == "undefined") {
	var Msg_Log = {};
	Msg_Log.SYNC_LOG_LOGINOUT = 180001; //后台登录退出流水
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
