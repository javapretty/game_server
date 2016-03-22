/*
*	描述：错误号管理
*	作者：张亚磊
*	时间：2016/03/14
*	qq:784599938
*	tel:18268193219
*/

if (typeof Error_Code == "undefined") {
	var Error_Code = {};

	Error_Code.ERROR_CLIENT_OPERATE 						= 10050;				//客户端非法操作
	Error_Code.ERROR_CLIENT_PARAM 							= 10051;				//客户端参数错误
	Error_Code.ERROR_CONFIG_NOT_EXIST 					= 10052;				//配置文件错误
	
	Error_Code.ERROR_ROLE_NOT_EXIST 						= 10100;				//角色名已存在
	Error_Code.ERROR_ROLE_NAME_EXIST 					= 10101;				//角色名已存在
	Error_Code.ERROR_ROLE_OFFLINE 							= 10102;				//玩家已离线
	Error_Code.ERROR_LEVEL_NOT_ENOUGH 					= 10103;				//角色等级不足
	Error_Code.ERROR_VITALITY_TIMES_NOT_ENOUGH	=	10104;				//购买体力次数不足
	Error_Code.ERROR_SKILL_POINT_NOT_ENOUGH			=	10105;				//技能点不足
	
	Error_Code.ERROR_BAG_FULL									= 10200,				//背包已满
	Error_Code.ERROR_ITEM_NOT_EXIST						= 10201;				//物品不存在
	Error_Code.ERROR_ITEM_NOT_ENOUGH						= 10202;				//物品不足
	Error_Code.ERROR_COPPER_NOT_ENOUGH					= 10203;				//铜钱不足
	Error_Code.ERROR_GOLD_NOT_ENOUGH						= 10204;				//元宝不足
}