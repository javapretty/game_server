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
	Data_Change.SHOP_CHANGE = 5;
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
