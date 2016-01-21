/*
 * Bag_Func.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef PACKER_FUNC_H_
#define PACKER_FUNC_H_

#include "Bag_Def.h"
#include "Bag_Struct.h"
#include "Msg_Define.h"

// 与Packer成员无关的函数放在这里
void merge_same_item(const std::vector<Item_Info> &item_vec_in, std::vector<Item_Info> &item_vec_out);

inline void pack_insert_item_to_msg(const Item_Info &item, MSG_520100 &msg) {
	switch (item.type) {
	case Item_Info::EQUIP:

		break;
	default:
		msg.item_info.push_back(item.item_basic);
		break;
	}
}

inline void pack_insert_item_to_msg(const Item_Info &item, MSG_300100 &item_msg, MSG_300101 &addit_msg, int offset = 0) {
	int new_index = item.item_basic.index + offset;
	switch (item.type) {
	case Item_Info::EQUIP:
		break;
	default:
		break;
	}
	item_msg.item_info.push_back(item.item_basic);
	item_msg.item_info.rbegin()->index = new_index;
}

void pack_merge_item_array(std::vector<Item_Info*> &item_array, Packer_Type pack_type,
		MERGE_WAY merge_way, UInt_Set &changed_set);
bool pack_merge_equal_item(Item_Info &item_des, Item_Info &item_src);
bool pack_merge_similar_item(Item_Info &item_des, Item_Info &item_src);

bool pack_is_req_item(const Item_Info &item, uint32_t id, Bind_Verify bind_verify = BIND_NONE);

int get_capacity_price(Packer_Type pack_type, int use_type, int start, int end, int &price);
void get_capacity_super_item_nums(int start, int end, int pack_type, int &item_nums);

inline int get_sub_type_by_money_type(Money_Type money_type, Money_Sub_Type &sub_type) {
	switch (money_type) {
	case BIND_COPPER:
		sub_type = BIND_COPPER_FIRST;
		break;
	case COPPER:
		sub_type = COPPER_ONLY;
		break;
	case COUPON:
		sub_type = COUPON_ONLY;
		break;
	case GOLD:
		sub_type = GOLD_ONLY;
		break;
	default:
		return ERROR_MONEY_TYPE_ILLEGAL;
	}
	return 0;
}

int get_item_color(const int item_id);

inline int get_full_error_code(const Packer_Type pack_type) {
	switch (pack_type) {
	case PACKER_T_EQUIP_INDEX:
		return ERROR_SERVER_INNER;
	case PACKER_T_PACKAGE_INDEX:
		return ERROR_PACK_FULL;
	default:
		return ERROR_SERVER_INNER;
	}
}

bool is_need_trade_pay_limit(const Money_Sub_Info &info);
bool is_need_trade_get_limit(const Money_Add_Info &info);

#endif /* PACKER_FUNC_H_ */
