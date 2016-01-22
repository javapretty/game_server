/*
* Bag_Config.cpp
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "Configurator.h"
#include "Mutex_Guard.h"
#include <string>

void Configurator::load_bag_config(void) {
	GUARD_WRITE(Config_Lock, mon, bag_config_.lock);

	std::string bag_path("config/bag/bag.json");
	load_json_file(bag_path.c_str(), bag_config_.bag.get_next_json());
	bag_config_.bag.update_version();

	bag_config_.item.clear_next_json();
	load_json_file("config/bag/item.json", bag_config_.item.get_next_json());
	bag_config_.item.cover_next_json_to_map();
	bag_config_.item.update_version();

	bag_config_.item_gift.clear_next_json();
	load_json_file("config/bag/gift.json", bag_config_.item_gift.get_next_json());
	bag_config_.item_gift.cover_next_json_to_map();
	bag_config_.item_gift.update_version();

	bag_config_.item_lottery.clear_next_json();
	load_json_file("config/bag/lottery.json", bag_config_.item_lottery.get_next_json());
	bag_config_.item_lottery.cover_next_json_to_map();
	bag_config_.item_lottery.update_version();

	bag_config_.item_prop.clear_next_json();
	load_json_file("config/bag/prop.json", bag_config_.item_prop.get_next_json());
	bag_config_.item_prop.cover_next_json_to_map();
	bag_config_.item_prop.update_version();
}

const Json::Value &Configurator::bag_config(void) {
	return get_json_value_with_rlock(bag_config_.lock, bag_config_.bag);
}

const Json::Value &Configurator::item(const int item_id){
	return get_json_value_with_rlock_in_map(bag_config_.lock, item_id, bag_config_.item);
}

const Json::Value &Configurator::item_gift(const int item_id) {
	return get_json_value_with_rlock_in_map(bag_config_.lock, item_id, bag_config_.item_gift);
}

const Json::Value &Configurator::item_lottery(const int item_id) {
	return get_json_value_with_rlock_in_map(bag_config_.lock, item_id, bag_config_.item_lottery);
}

const Json::Value &Configurator::item_prop(const int item_id) {
	return get_json_value_with_rlock_in_map(bag_config_.lock, item_id, bag_config_.item_prop);
}
