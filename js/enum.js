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
	Role_Status.ROLE_SUCCESS_LOAD 		= 1;		//加载成功
	Role_Status.ROLE_SUCCESS_CREATE 	= 2;		//创建成功
	Role_Status.ROLE_NOT_EXIST				= 3;		//角色不存在
	Role_Status.ROLE_HAS_EXIST				= 4;		//	角色已经存在
	Role_Status.ROLE_SAVE_OFFLINE		= 5;		//角色下线保存
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
