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
	Property_Type.PHYSICAL_ATTACK = 9;//物理攻击
	Property_Type.MAGIC_ATTACK = 10;		//魔法攻击
	Property_Type.PHYSICAL_DEF = 11;		//物理防御
	Property_Type.MAGIC_DEF = 12;			//魔法防御
	Property_Type.CRIT = 13;					//暴击
}