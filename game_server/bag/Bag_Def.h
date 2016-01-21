/*
 * Bag_Def.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef PACKER_DEF_H_
#define PACKER_DEF_H_

const int PACK_INDEX_GAP = 10000;

// 一行有7个背包格子
const int PACKAGE_GRID_PER_LINE = 7;
const int STORAGE_GRID_PER_LINE = 8;

enum MERGE_WAY {
	MERGE_WAY_EQUAL,
	MERGE_WAY_SIMILAR
};

// 指定包裹类型
enum Packer_Type {
	PACKER_T_EQUIP_INDEX 			= 10000,	//人物装备
	PACKER_T_PACKAGE_INDEX 		= 20000,	//人物背包
	PACKER_T_STORAGE_INDEX 		= 30000,	//人物背包
	PACKER_T_ALL_INDEX = 0
};

// 增加money bind_copper/copper/...
enum Money_Type {
	BIND_COPPER = 1,
	COPPER = 2,
	COUPON = 3,
	GOLD = 4,
	MONEY_TYPE_END
};

// 使用money的方式
enum Money_Sub_Type {
	BIND_COPPER_FIRST = 1,
	COPPER_ONLY = 2,
	BIND_COPPER_ONLY = 3,
	COUPON_ONLY = 4,
	GOLD_ONLY = 5,
};

// 若已经调用了TRY的接口了，则不需要在内部重复调用
enum Pack_Try {
	WITH_TRY = 0,
	WITHOUT_TRY = 1
};

enum Money_Opt_Type {
	MONEY_OPT_TRY,
	MONEY_OPT_REAL,
};

enum {
	MAX_LOCK_SEC = 5
};

// 由于存在交易的情况，不能依据SEQ是否为0来判断是否需要生成SEQ，故用枚举指定是否生成SEQ
enum Seq_Type {
	GENERATE_SEQ,
	DONT_GENERATE_SEQ
};

#endif /* PACKER_DEF_H_ */
