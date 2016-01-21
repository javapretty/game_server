/*
 * Bag.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef PACKER_H_
#define PACKER_H_

#include "Bag_Struct.h"
#include "Msg_Define.h"

class Game_Player;
class Bag {
public:
	Bag(void);
	virtual ~Bag(void);

	void reset(void);
	int time_up(Time_Value &now);
	void recovery(void);
	void clear_item(Bag_Type bag_type);

	int load_detail(Player_Data &data);
	int save_detail(Player_Data &data);
	int init(Game_Player *player);

	inline const Money_Info &bag_get_money(void) { return bag_info_.money_info; }

	// msg
	int bag_fetch_bag_info(const MSG_120100 &msg);
	int bag_add_capacity(const MSG_120101 &msg);
	int bag_discard_item(const MSG_120102 &msg);
	int bag_move_item(const MSG_120103 &msg);
	int bag_split_item(const MSG_120104 &msg);
	int bag_sort_item(const MSG_120105 &msg);
	int bag_merge_item(const MSG_120106 &msg);
	int bag_sell_item(const MSG_120107 &msg);

	// get item info
	int bag_get_item_id(const uint32_t index, uint32_t &id);
	int bag_calc_item(const Bag_Type bag_type);
	int bag_calc_item(const Bag_Type bag_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE);
	int bag_calc_unlock_item(const Bag_Type bag_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE);
	int bag_calc_item(const uint32_t index);
	inline bool is_item_exist(uint32_t index);
	void bag_get_indexes(const Bag_Type bag_type, const uint32_t id, Int_Vec &indexes);

	// insert
	int bag_try_insert_item(const Bag_Type bag_type, const Item_Info &item);
	int bag_insert_item(const Bag_Type bag_type, const Item_Info &item, Pack_Try bag_try = WITH_TRY);
	int bag_try_insert_item(const Bag_Type bag_type, const std::vector<Item_Info> &item_list);
	int bag_insert_item(const Bag_Type bag_type, const std::vector<Item_Info> &item_list, Pack_Try bag_try = WITH_TRY);
	// erase by bag_type id amount
	int bag_try_erase_item(const Bag_Type bag_type, const Id_Amount &id_amount);
	int bag_erase_item(const Bag_Type bag_type, const Id_Amount &id_amount, Pack_Try bag_try = WITH_TRY);
	int bag_try_erase_item(const Bag_Type bag_type, const std::vector<Id_Amount> &id_amount_list);
	int bag_erase_item(const Bag_Type bag_type, const std::vector<Id_Amount> &id_amount_list, Pack_Try bag_try = WITH_TRY);
	// erase by index amount
	int bag_try_erase_item(const Index_Amount &index_amount);
	int bag_erase_item(const Index_Amount &index_amount, Pack_Try bag_try = WITH_TRY);
	int bag_try_erase_item(const std::vector<Index_Amount> &index_amount_list);
	int bag_erase_item(const std::vector<Index_Amount> &index_amount_list, Pack_Try bag_try = WITH_TRY);
	// erase by index (discard)
	int bag_try_erase_item(const uint32_t index);
	int bag_erase_item(const uint32_t index, Pack_Try bag_try = WITH_TRY);
	int bag_try_erase_item(const UInt_Vec &index_list);
	int bag_erase_item(const UInt_Vec &index_list, Pack_Try bag_try = WITH_TRY);

	// move
	int bag_try_move_item(const uint32_t index_from, const uint32_t index_to);
	int bag_move_item(const uint32_t index_from, const uint32_t index_to, Pack_Try bag_try = WITH_TRY);
	int bag_try_move_item(const uint32_t index_from, const Bag_Type bag_type_to);
	int bag_move_item(const uint32_t index_from, const Bag_Type bag_type_to, Pack_Try bag_try = WITH_TRY);
	int bag_move_item(const uint32_t index_from, const Bag_Type bag_type_to, UInt_Set &changed, Pack_Try bag_try = WITH_TRY);
	int bag_try_move_item(const Bag_Type bag_type_from, const Bag_Type bag_type_to);
	int bag_move_item(const Bag_Type bag_type_from, const Bag_Type bag_type_to, Pack_Try bag_try = WITH_TRY);
	// 尽力移动每个物品格 如果没有一个物品被移动则认为是失败
	int bag_attempt_move_everyitem(const Bag_Type bag_type_from, const Bag_Type bag_type_to, int &move_item_nums);

	int bag_try_add_money(const Money_Add_Info &info);
	int bag_add_money(const Money_Add_Info &info, Money_Opt_Type type = MONEY_OPT_REAL);
	int bag_try_add_money(const std::vector<Money_Add_Info> &money_add_list);
	int bag_add_money(const std::vector<Money_Add_Info> &money_add_list, Money_Opt_Type type = MONEY_OPT_REAL);

	// 返回能购买的数量
	int bag_get_buy_power(const Money_Sub_Type sub_type, int unit_price);

	int bag_try_sub_money(const Money_Sub_Info &info);
	int bag_sub_money(const Money_Sub_Info &info, Money_Opt_Type type = MONEY_OPT_REAL);
	int bag_try_sub_money(const std::vector<Money_Sub_Info> &money_sub_list);
	int bag_sub_money(const std::vector<Money_Sub_Info> &money_sub_list, Money_Opt_Type type = MONEY_OPT_REAL);

	int bag_is_bag_type(const int32_t index, Bag_Type bag_type);
	int bag_get_bag_type(const int32_t index, Bag_Type &bag_type);

	// 按金钱扣除类型加解锁
	inline void lock_money_by_sub_type(Money_Sub_Type type);
	inline void unlock_money_by_sub_type(Money_Sub_Type type);
	// lock
	void lock_money(Money_Type type);
	void lock_item(uint32_t index);

	void unlock_money(Money_Type type);
	void unlock_item(uint32_t index);

	inline void set_change(void);

	// 剩余格数
	inline int bag_get_remain_capacity(const Bag_Type bag_type) {
		return bag_get_capacity(bag_type) - bag_get_size(bag_type);
	}
	int bag_get_size(const Bag_Type bag_type);

	const Item_Info *bag_get_const_item(const uint32_t index);

protected:
	Item_Info *bag_get_item(const uint32_t index);

	int bag_get_capacity(const Bag_Type bag_type);

	int bag_sort_item(const Bag_Type bag_type, MERGE_WAY merge_way);

	// active informing
	// money
	int bag_active_update_money(void);
	// capacity
	int bag_active_update_capacity(const Bag_Type bag_type);
	// item
	int bag_active_update_item_detail(const UInt_Set *changed_index);

private:
	// 确定背包的容积是否足够(不叠放情况下)
	int bag_try_insert_to_empty_index_direct(const Bag_Type bag_type, const Item_Info &item);
	// 向背包中放入批量物品，先尝试叠放，再尝试放入新的格子中
	int bag_insert_to_exist_index_first(const Bag_Type bag_type, const std::vector<Item_Info> &item_list,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try bag_try = WITH_TRY);
	// 向背包中放入物品，先尝试叠放，再尝试放入新的格子中
	int bag_insert_to_exist_index_first(const Bag_Type bag_type, const Item_Info &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try bag_try = WITH_TRY);
	// 向背包中放入物品，只放入新的格子中
	int bag_insert_to_empty_index_direct(const Bag_Type bag_type, const Item_Info &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try bag_try = WITH_TRY);

	// 移除物品不指定位置（若移除0个或负数个物品，返回成功，但不做任何操作）
	int bag_erase_item(const Bag_Type bag_type, const Id_Amount &id_amount, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);
	// 移除物品不指定位置（批量）
	int bag_erase_item(const Bag_Type bag_type, const std::vector<Id_Amount> &id_amount_list, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);
	// 移除指定位置指定数量物品
	int bag_erase_item(const Index_Amount &index_amount, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);
	// 移除指定位置指定数量物品（批量）
	int bag_erase_item(const std::vector<Index_Amount> &index_amount_list, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);
	// 移除指定位置所有物品
	int bag_erase_item(const uint32_t index, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);
	// 移除指定位置所有物品（批量）
	int bag_erase_item(const UInt_Vec &index_list, UInt_Set *changed_set, Pack_Try bag_try = WITH_TRY);

	int bag_set_money(const Money_Info &money_info, Money_Opt_Type type);

	inline void bag_get_iter(const Bag_Type bag_type, Bag_Info::Item_Map::iterator &begin, Bag_Info::Item_Map::iterator &end);
	inline void bag_get_index(const Bag_Type bag_type, uint32_t &begin, uint32_t &end);

	inline bool is_money_lock(Money_Type type);
	inline bool is_item_lock(uint32_t index);

	inline bool is_money_lock(void);
	inline bool is_item_lock(void);

	inline Item_Info *bag_get_item_pointer(const uint32_t index);

private:
	Game_Player *player_;
	uint32_t seq_generator_;
	Bag_Info bag_info_;
};

inline bool Bag::is_money_lock(Money_Type type) {
	return bag_info_.money_lock_map.count(type);
}

inline bool Bag::is_item_lock(uint32_t index) {
	return bag_info_.item_lock_map.count(index);
}

inline bool Bag::is_money_lock(void) {
	return !bag_info_.money_lock_map.empty();
}

inline bool Bag::is_item_lock(void) {
	return !bag_info_.item_lock_map.empty();
}

inline bool Bag::is_item_exist(uint32_t index) {
	return bag_info_.item_map.count(index);
}

inline void Bag::set_change(void) {
	bag_info_.is_change_ = true;
}

inline Item_Info *Bag::bag_get_item_pointer(const uint32_t index) {
	Bag_Info::Item_Map::iterator it = bag_info_.item_map.find(index);
	if (it != bag_info_.item_map.end()) {
		return it->second;
	}
	return NULL;
}

inline void Bag::lock_money_by_sub_type(Money_Sub_Type type) {
	switch (type) {
	case BIND_COPPER_FIRST:
		this->lock_money(BIND_COPPER);
		this->lock_money(COPPER);
		break;
	case COPPER_ONLY:
		this->lock_money(COPPER);
		break;
	case BIND_COPPER_ONLY:
		this->lock_money(BIND_COPPER);
		break;
	case COUPON_ONLY:
		this->lock_money(COUPON);
		break;
	case GOLD_ONLY:
		this->lock_money(GOLD);
		break;
	default:
		MSG_USER_TRACE("wrong Money_Sub_Type : %d", type);
		break;
	}
}

inline void Bag::unlock_money_by_sub_type(Money_Sub_Type type) {
	switch (type) {
	case BIND_COPPER_FIRST:
		this->unlock_money(BIND_COPPER);
		this->unlock_money(COPPER);
		break;
	case COPPER_ONLY:
		this->unlock_money(COPPER);
		break;
	case BIND_COPPER_ONLY:
		this->unlock_money(BIND_COPPER);
		break;
	case COUPON_ONLY:
		this->unlock_money(COUPON);
		break;
	case GOLD_ONLY:
		this->unlock_money(GOLD);
		break;
	default:
		MSG_USER_TRACE("wrong Money_Sub_Type : %d", type);
		break;
	}
}

#endif /* PACKER_H_ */
