/*
*	描述：枚举类型管理
*	作者：张亚磊
*	时间：2016/02/24
*/

if (typeof Server_Type == "undefined") {
	var Server_Type = {};
	Server_Type.GAME_SERVER = 1;
	Server_Type.MASTER_SERVER = 2;
}

if (typeof Role_Status == "undefined") {
	var Role_Status = {};
	Role_Status.SUCCESS_LOADED 		= 1;		//加载成功
	Role_Status.SUCCESS_CREATED 		= 2;		//创建成功
	Role_Status.ROLE_NOT_EXIST			= 3;		//角色不存在
	Role_Status.ROLE_HAS_EXIST			= 4;		//	角色已经存在
	Role_Status.ROLE_SAVE_OFFLINE	= 5;		//角色下线保存
}

if (typeof Property_Type == "undefined") {
	var Property_Type = {};
	Property_Type.POWER = 1;					//力量
	Property_Type.BRAINS = 2;					//智力
	Property_Type.AGILE = 3;					//敏捷
	Property_Type.POWER_GROW = 4;			//力量成长
	Property_Type.BRAINS_GROW = 5;			//智力成长
	Property_Type.AGILE_GROW = 6;			//敏捷成长
	Property_Type.HP = 7;							//当前生命
	Property_Type.HP_MAX = 8;					//最大生命
	Property_Type.ANGER = 9;					//当前怒气
	Property_Type.ANGER_MAX =10;				//最大怒气
	Property_Type.PHYSICAL_ATTACK = 11;//物理攻击
	Property_Type.MAGIC_ATTACK = 12;		//魔法攻击
	Property_Type.PHYSICAL_DEF = 13;		//物理防御
	Property_Type.MAGIC_DEF = 14;			//魔法防御
	Property_Type.CRIT = 15;					//暴击
}

if (typeof Shop_Type == "undefined") {
	var Shop_Type = {};
	Shop_Type.COPPER_SHOP = 1; //铜币商店
	Shop_Type.GOLD_SHOP = 2; //金币商店
}

if (typeof Rank_Type == "undefined") {
	var Rank_Type = {};
	Rank_Type.LEVEL_RANK = 1; //等级排行
	Rank_Type.BATTLE_RANK = 2; //战力排行
}
