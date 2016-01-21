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

	static int create_init(Player_Data &data);
	void reset(void);
	int time_up(Time_Value &now);
	void recovery(void);
	void clear_item(Packer_Type pack_type);

	int load_detail(Player_Data &data);
	int save_detail(Player_Data &data);
	int init(Game_Player *player);

	// msg
	int pack_fetch_bag_info(const MSG_120100 &msg);
	int pack_add_capacity(const MSG_120101 &msg);
	int pack_discard_item(const MSG_120102 &msg);
	int pack_move_item(const MSG_120103 &msg);
	int pack_split_item(const MSG_120104 &msg);
	int pack_sort_item(const MSG_120105 &msg);
	int pack_merge_item(const MSG_120108 &msg);
	int pack_sell_item(const MSG_120109 &msg);

	// 客户端要求查看别人的装备信息用偏移量区分
	int pack_fetch_other_equip_info(Block_Buffer &basic_buf, Block_Buffer &addit_buf, int offset);

	// get item info
	int pack_get_item_id(const uint32_t index, uint32_t &id);
	int pack_calc_item(const Packer_Type pack_type);
	int pack_calc_item(const Packer_Type pack_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE);
	int pack_calc_unlock_item(const Packer_Type pack_type, const uint32_t id, Bind_Verify bind_verify = BIND_NONE);
	int pack_calc_item(const uint32_t index);
	inline bool is_item_exist(uint32_t index);
	void get_equip_index(const Packer_Type pack_type, Int_Vec &equip_index);
	void pack_get_indexes(const Packer_Type pack_type, const uint32_t id, Int_Vec &indexes);

	// insert
	// 指定index插入装备
	int pack_insert_equip(const uint32_t index, const Item_Info &item);
	int pack_try_insert_item(const Packer_Type pack_type, const Item_Info &item);
	int pack_insert_item(const Packer_Type pack_type, const Item_Info &item, Pack_Try pack_try = WITH_TRY);
	int pack_try_insert_item(const Packer_Type pack_type, const std::vector<Item_Info> &item_list);
	int pack_insert_item(const Packer_Type pack_type, const std::vector<Item_Info> &item_list, Pack_Try pack_try = WITH_TRY);
	// erase by pack_type id amount
	int pack_try_erase_item(const Packer_Type pack_type, const Id_Amount &id_amount);
	int pack_erase_item(const Packer_Type pack_type, const Id_Amount &id_amount, Pack_Try pack_try = WITH_TRY);
	int pack_try_erase_item(const Packer_Type pack_type, const std::vector<Id_Amount> &id_amount_list);
	int pack_erase_item(const Packer_Type pack_type, const std::vector<Id_Amount> &id_amount_list, Pack_Try pack_try = WITH_TRY);
	// erase by index amount
	int pack_try_erase_item(const Index_Amount &index_amount);
	int pack_erase_item(const Index_Amount &index_amount, Pack_Try pack_try = WITH_TRY);
	int pack_try_erase_item(const std::vector<Index_Amount> &index_amount_list);
	int pack_erase_item(const std::vector<Index_Amount> &index_amount_list, Pack_Try pack_try = WITH_TRY);
	// erase by index (discard)
	int pack_try_erase_item(const uint32_t index);
	int pack_erase_item(const uint32_t index, Pack_Try pack_try = WITH_TRY);
	int pack_try_erase_item(const UInt_Vec &index_list);
	int pack_erase_item(const UInt_Vec &index_list, Pack_Try pack_try = WITH_TRY);

	// move
	int pack_try_move_item(const uint32_t index_from, const uint32_t index_to);
	int pack_move_item(const uint32_t index_from, const uint32_t index_to, Pack_Try pack_try = WITH_TRY);
	int pack_try_move_item(const uint32_t index_from, const Packer_Type pack_type_to);
	int pack_move_item(const uint32_t index_from, const Packer_Type pack_type_to, Pack_Try pack_try = WITH_TRY);
	int pack_move_item(const uint32_t index_from, const Packer_Type pack_type_to, UInt_Set &changed, Pack_Try pack_try = WITH_TRY);
	int pack_try_move_item(const Packer_Type pack_type_from, const Packer_Type pack_type_to);
	int pack_move_item(const Packer_Type pack_type_from, const Packer_Type pack_type_to, Pack_Try pack_try = WITH_TRY);
	// 尽力移动每个物品格 如果没有一个物品被移动则认为是失败

	int pack_attempt_move_everyitem(const Packer_Type pack_type_from, const Packer_Type pack_type_to, int &move_item_nums);

	// 此接口还是保留
	const Money_Info &pack_get_money(void) {
		return bag_info_.money_info;
	}

	int pack_try_add_money(const Money_Add_Info &info);
	int pack_add_money(const Money_Add_Info &info, Money_Opt_Type type = MONEY_OPT_REAL);
	int pack_try_add_money(const Money_Add_List &money_add_list);
	int pack_add_money(const Money_Add_List &money_add_list, Money_Opt_Type type = MONEY_OPT_REAL);

	// 返回能购买的数量
	int pack_get_buy_power(const Money_Sub_Type sub_type, int unit_price);

	int pack_try_sub_money(const Money_Sub_Info &info);
	int pack_sub_money(const Money_Sub_Info &info, Money_Opt_Type type = MONEY_OPT_REAL);
	int pack_try_sub_money(const Money_Sub_List &money_sub_list);
	int pack_sub_money(const Money_Sub_List &money_sub_list, Money_Opt_Type type = MONEY_OPT_REAL);

	int pack_is_pack_type(const int32_t index, Packer_Type pack_type);
	int pack_get_pack_type(const int32_t index, Packer_Type &pack_type);

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
	inline int pack_get_remain_capacity(const Packer_Type pack_type) {
		return pack_get_capacity(pack_type) - pack_get_size(pack_type);
	}
	int pack_get_size(const Packer_Type pack_type);

	inline void set_last_wipe_time(void);
	inline const Time_Value &get_last_wipe_time(void);

	const Item_Info *pack_get_const_item(const uint32_t index);

protected:
	Item_Info *pack_get_item(const uint32_t index);

	int pack_get_capacity(const Packer_Type pack_type);

	int pack_sort_item(const Packer_Type pack_type, MERGE_WAY merge_way);

	// active informing
	// money
	int pack_active_update_money(void);
	// capacity
	int pack_active_update_capacity(const Packer_Type pack_type);
	// item
	int pack_active_update_item_detail(const UInt_Set *changed_index);

private:
	// 确定背包的容积是否足够(不叠放情况下)
	int pack_try_insert_to_empty_index_direct(const Packer_Type pack_type, const Item_Info &item);
	// 向背包中放入批量物品，先尝试叠放，再尝试放入新的格子中
	int pack_insert_to_exist_index_first(const Packer_Type pack_type, const std::vector<Item_Info> &item_list,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);
	// 向背包中放入物品，先尝试叠放，再尝试放入新的格子中
	int pack_insert_to_exist_index_first(const Packer_Type pack_type, const Item_Info &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);
	// 向背包中放入物品，只放入新的格子中
	int pack_insert_to_empty_index_direct(const Packer_Type pack_type, const Item_Info &item,
			UInt_Set *changed_set, Seq_Type seq_type, Pack_Try pack_try = WITH_TRY);

	// 移除物品不指定位置（若移除0个或负数个物品，返回成功，但不做任何操作）
	int pack_erase_item(const Packer_Type pack_type, const Id_Amount &id_amount, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除物品不指定位置（批量）
	int pack_erase_item(const Packer_Type pack_type, const std::vector<Id_Amount> &id_amount_list, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置指定数量物品
	int pack_erase_item(const Index_Amount &index_amount, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置指定数量物品（批量）
	int pack_erase_item(const std::vector<Index_Amount> &index_amount_list, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置所有物品
	int pack_erase_item(const uint32_t index, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);
	// 移除指定位置所有物品（批量）
	int pack_erase_item(const UInt_Vec &index_list, UInt_Set *changed_set, Pack_Try pack_try = WITH_TRY);

	int pack_set_money(const Money_Info &money_info, Money_Opt_Type type);

	inline void pack_get_iter(const Packer_Type pack_type, Bag_Info::Item_Map::iterator &begin, Bag_Info::Item_Map::iterator &end);
	inline void pack_get_index(const Packer_Type pack_type, uint32_t &begin, uint32_t &end);

	inline bool is_money_lock(Money_Type type);
	inline bool is_item_lock(uint32_t index);

	inline bool is_money_lock(void);
	inline bool is_item_lock(void);

	inline Item_Info *pack_get_item_pointer(const uint32_t index);

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

inline Item_Info *Bag::pack_get_item_pointer(const uint32_t index) {
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

inline void Bag::set_last_wipe_time(void) {
	bag_info_.last_wipe_time = Time_Value::gettimeofday();
}

inline const Time_Value &Bag::get_last_wipe_time(void) {
	return bag_info_.last_wipe_time;
}

#endif /* PACKER_H_ */
