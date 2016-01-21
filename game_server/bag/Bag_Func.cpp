/*
 * Bag_Func.cpp
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "Bag_Func.h"
#include "Configurator.h"
#include "Log.h"

void bag_merge_item_array(std::vector<Item_Info*> &item_array, Bag_Type bag_type,
		MERGE_WAY merge_way, UInt_Set &changed_set) {
	const uint32_t array_size = item_array.size();
	// 模拟人的的操作，不停将道具从index_now合并至index_pre
	uint32_t index_pre = 0;
	uint32_t index_now = 1;
	uint32_t index_flag = 1 + bag_type;		// 标记排序后的道具所在的index
	for (index_pre = 0; index_now < array_size; ++index_now) {
		bool merged = false;
		if (merge_way == MERGE_WAY_SIMILAR) {
			merged = bag_merge_similar_item(*item_array[index_pre], *item_array[index_now]);
		} else {
			merged = bag_merge_equal_item(*item_array[index_pre], *item_array[index_now]);
		}

		if (merged) {
			// 如果合并了，那么前后2个位置均改变了
			changed_set.insert(item_array[index_pre]->item_basic.index);
			changed_set.insert(item_array[index_now]->item_basic.index);
			if (item_array[index_now]->item_basic.amount <= 0) {
				continue;
			}
		}
		// 如果次序不对
		if (item_array[index_pre]->item_basic.index != index_flag) {
			changed_set.insert(item_array[index_pre]->item_basic.index);
			item_array[index_pre]->item_basic.index = index_flag;
			changed_set.insert(index_flag);
		}
		index_pre = index_now;
		index_flag++;
	}

	if (index_pre < array_size) {
		// 如果次序不对
		if (item_array[index_pre]->item_basic.index != index_flag) {
			changed_set.insert(item_array[index_pre]->item_basic.index);
			item_array[index_pre]->item_basic.index = index_flag;
			changed_set.insert(index_flag);
		}
	}
}

bool bag_merge_equal_item(Item_Info &item_des, Item_Info &item_src) {
	int pre_remain_space = item_des.get_item_remain_amount();
	if (is_equal_item(item_des, item_src) && pre_remain_space  > 0) {
		int32_t item_src_amount = item_src.item_basic.amount;
		int32_t item_src_remain = std::max(0, item_src_amount - pre_remain_space);
		item_src.item_basic.amount = item_src_remain;
		item_des.item_basic.amount += std::min(item_src_amount, pre_remain_space);
		return true;
	}

	return false;
}

bool bag_merge_similar_item(Item_Info &item_des, Item_Info &item_src) {
	int pre_remain_space = item_des.get_item_remain_amount();
	if (is_similar_item(item_des, item_src) && pre_remain_space > 0) {
		int32_t item_src_amount = item_src.item_basic.amount;
		int32_t item_src_remain = std::max(0, item_src_amount - pre_remain_space);
		item_src.item_basic.amount = item_src_remain;
		item_des.item_basic.amount += std::min(item_src_amount, pre_remain_space);
		// 合并时，只要有一个是绑定的，则物品数量较多的格子全部转换为绑定的
		if (item_des.item_basic.bind == Item_Info::BIND || item_src.item_basic.bind == Item_Info::BIND) {
			// 若不是只是有一个物品为绑定的，则物品数量较少的格子为不绑定
			if (item_des.item_basic.bind != item_src.item_basic.bind) {
				item_src.item_basic.bind = Item_Info::UNBIND;
			}
			item_des.item_basic.bind = Item_Info::BIND;
		}
		return true;
	}

	return false;
}

bool bag_is_req_item(const Item_Info &item, uint32_t id, Bind_Verify bind_verify) {
	switch (bind_verify) {
	case BIND_NONE:
		if (item.item_basic.id == id) {
			return true;
		} else {
			return false;
		}
		/* no break */
	case UNBIND_ONLY:
		if (item.item_basic.id == id &&
				(item.item_basic.bind == Item_Info::UNBIND || item.item_basic.bind == Item_Info::USED_BIND)) {
			return true;
		} else {
			return false;
		}
		/* no break */
	case BIND_ONLY:
		if (item.item_basic.id == id && item.item_basic.bind == Item_Info::BIND) {
			return true;
		} else {
			return false;
		}
		/* no break */
	}
	return false;
}

void merge_same_item(const std::vector<Item_Info>& item_vec_in, std::vector<Item_Info>& item_vec_out) {
	item_vec_out.clear();
	// 合并可重复item
	boost::unordered_map<Item_Info, int32_t> amount_map;
	for (std::vector<Item_Info>::const_iterator it = item_vec_in.begin(); it != item_vec_in.end(); ++it) {
		amount_map[*it] += it->item_basic.amount;
	}

	for (boost::unordered_map<Item_Info, int32_t>::const_iterator it = amount_map.begin(); it != amount_map.end(); ++it) {
		Item_Info item_temp(it->first);
		item_temp.item_basic.amount = it->second;
		item_vec_out.push_back(item_temp);
	}
}

void get_capacity_super_item_nums(int start, int end, int bag_type, int &item_nums) {
	item_nums = 0;
	if (start >= end) {
		return;
	}
	int gap = end - start;
	int grid_per_line = 0;
	if (bag_type == BAG_T_BAG_INDEX) {
		grid_per_line = BAG_GRID_PER_LINE;
	} else {
		grid_per_line = STORAGE_GRID_PER_LINE;
	}
	int item_nums_temp = gap / grid_per_line;
	if (gap % grid_per_line != 0) {
		++item_nums_temp;
	}
	item_nums = item_nums_temp;
}

int get_capacity_price(Bag_Type bag_type, int use_type, int start, int end, int &price) {
	return 0;
}
