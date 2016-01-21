/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Configurator.h"
#include "DB_Operator.h"
#include "Game_Manager.h"


const Time_Value Recycle_Tick::recycle_time_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::tick_interval_ = Time_Value(2, 0);

Account_Info::Account_Info(void) { reset(); }

int Account_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	return 0;
}

int Account_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_string(account);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	return 0;
}

void Account_Info::reset(void) {
	account.clear();
	agent_num = 0;
	server_num = 0;
}

Gate_Player_Info::Gate_Player_Info(void) { reset(); }

int Gate_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	return 0;
}

int Gate_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	return 0;
}

void Gate_Player_Info::reset(void) {
	role_id = 0;
	account.clear();
}

Master_Player_Info::Master_Player_Info(void) { reset(); }

int Master_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(level);
	return 0;
}

int Master_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_int32(level);
	return 0;
}

void Master_Player_Info::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	level = 0;
}

Game_Player_Info::Game_Player_Info(void) { reset(); }

int Game_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_uint8(gender);
	buffer.write_int32(level);
	buffer.write_uint8(career);

	buffer.write_int32(create_time);
	buffer.write_int32(last_sign_in_time);
	buffer.write_int32(last_sign_out_time);
	buffer.write_string(ip);

	return 0;
}

int Game_Player_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_uint8((uint8_t &)gender);
	buffer.read_int32(level);
	buffer.read_uint8((uint8_t &)career);
	buffer.read_int32(create_time);
	buffer.read_int32(last_sign_in_time);
	buffer.read_int32(last_sign_out_time);
	buffer.read_string(ip);
	return 0;
}

int Game_Player_Info::load(void) {
	return CACHED_INSTANCE->load_player_info(*this);
}

int Game_Player_Info::save(void) {
	return CACHED_INSTANCE->save_player_info(*this);
}

void Game_Player_Info::reset(void) {
	role_id = 0;
	agent_num = 0;
	server_num = 0;
	account.clear();
	role_name.clear();
	gender = 0;
	career = 0;
	create_time = 0;
	level = 0;
	last_sign_in_time = 0;
	last_sign_out_time = 0;
	ip.clear();
}

int Money_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int32(bind_copper);
	buffer.write_int32(copper);
	buffer.write_int32(coupon);
	buffer.write_int32(gold);

	buffer.write_int32(today_gold_pay);
	buffer.write_int32(today_gold_get);
	buffer.write_int32(today_copper_pay);
	buffer.write_int32(today_copper_get);

	return 0;
}

int Money_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int32(bind_copper);
	buffer.read_int32(copper);
	buffer.read_int32(coupon);
	buffer.read_int32(gold);

	buffer.read_int32(today_gold_pay);
	buffer.read_int32(today_gold_get);
	buffer.read_int32(today_copper_pay);
	buffer.read_int32(today_copper_get);

	return 0;
}

void Money_Info::reset(void) {
	bind_copper = 0;
	copper = 0;
	coupon = 0;
	gold = 0;

	today_gold_pay = 0;
	today_gold_get = 0;
	today_copper_pay = 0;
	today_copper_get = 0;
}

Money Money_Info::money(void) const {
	Money money;
	money.bind_copper = bind_copper;
	money.copper = copper;
	money.coupon = coupon;
	money.gold = gold;

	return money;
}

Item_Info::Item_Info(void) {
	reset();
}

Item_Info::Item_Info(uint32_t id, int32_t amount, uint8_t bind) {
	reset();
	item_basic.id = id;
	item_basic.amount = amount;
	item_basic.bind = bind;
	init();
}

Item_Info::Item_Info(const Item_Basic_Info &item) {
	reset();
	item_basic = item;
	init();
}

Item_Info::~Item_Info() {}

int Item_Info::init() {
	switch (item_basic.bind) {
	case UNBIND:
	case USED_BIND:
		item_basic.bind = UNBIND;
		break;
	// 若策划配置错了，物品为绑定
	default:
		item_basic.bind = BIND;
		break;
	}

	get_item_type(item_basic.id, type);
	if (type == EQUIP) {
	} else {
		memset(&addon, 0x00, sizeof (addon));
	}
	return 0;
}

int Item_Info::serialize(Block_Buffer &buffer) const {
	item_basic.serialize(buffer);
	buffer.write_int32(type);
	switch (type) {
	case EQUIP:
		break;
	default:
		break;
	}

	return 0;
}

int Item_Info::deserialize(Block_Buffer &buffer){
	reset();
	item_basic.deserialize(buffer);
	int type_value = 0;
	buffer.read_int32(type_value);
	type = static_cast<Item_Type>(type_value);
	switch (type) {
	case EQUIP:
		break;
	default:
		break;
	}

	return 0;
}

void Item_Info::reset(void){
	item_basic.reset();
	type = NORMAL;
	memset(&addon, 0x00, sizeof(addon));
}

int Item_Info::is_item_type(const uint32_t item_id, Item_Type item_type) {
	bool is_infer_type = false;
	if (EQUIP_START <= item_id && item_id < EQUIP_END) {
		is_infer_type = (item_type == EQUIP);
	}
	return is_infer_type? 0 : -1;
}

int Item_Info::get_item_type(const uint32_t item_id, Item_Type &item_type) {
	if (EQUIP_START <= item_id && item_id < EQUIP_END) {
		item_type = EQUIP;
	} else {
		item_type = NORMAL;
	}
	return 0;
}

void Item_Info::set_basic(const uint32_t index, const uint32_t id, const int32_t amount, const uint8_t bind) {
	item_basic.index = index;
	item_basic.id = id;
	item_basic.amount = (amount > 0)? amount : 0;
	item_basic.bind = bind;
	// get item type
	init();
}

int32_t Item_Info::get_item_stack_upper(const uint32_t item_id) {
	//if (CONFIG_INSTANCE->item(item_id)["overlap"].asInt() == 1) return 1;
	// 若以后服务端客户端用不同的配置文件，此判断可以去掉，只保留一个机制（配置overlap），不引入如果是XX，那么堆叠是XX的补丁式代码
	if (is_item_type(item_id, EQUIP) == 0) return 1;
	return 100;
}

bool Item_Info::operator == (const Item_Info &cmp) const {
	if (is_equal_item(*this, cmp)) {
		return true;
	} else {
		return false;
	}
}

bool operator<(const Item_Info &item1, const Item_Info &item2) {
	int sort1 = 0; //CONFIG_INSTANCE->item(item1.item_basic.id)["sort"].asInt();
	int sort2 = 1; //CONFIG_INSTANCE->item(item2.item_basic.id)["sort"].asInt();
	if (sort1 < sort2) {
		return true;
	} else if (sort1 == sort2 && item1.item_basic.id > item2.item_basic.id) {
		return true;
	} else if (sort1 == sort2 && item1.item_basic.id == item2.item_basic.id && item1.item_basic.bind > item2.item_basic.bind) {
		return true;
	} else if (sort1 == sort2 && item1.item_basic.id == item2.item_basic.id && item1.item_basic.bind == item2.item_basic.bind && item1.item_basic.index < item2.item_basic.index) {
		return true;
	} else {
		return false;
	}
}

const uint16_t Bag_Info::Capacity::equip_cap = 19;

Bag_Info::Bag_Info(void):money_lock_map(16), item_lock_map(16) { reset(); }

int Bag_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_uint16(capacity.pack_cap);
	buffer.write_uint16(capacity.storage_cap);
	money_info.serialize(buffer);

	uint16_t item_count = item_map.size();
	buffer.write_uint16(item_count);
	for (Item_Map::const_iterator it = item_map.begin(); it != item_map.end(); ++it) {
		it->second->serialize(buffer);
	}
	buffer.write_int64(last_wipe_time.sec());
	buffer.write_int8((int8_t)is_change_);
	return 0;
}

int Bag_Info::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_uint16(capacity.pack_cap);
	buffer.read_uint16(capacity.storage_cap);
	money_info.deserialize(buffer);

	uint16_t item_count = 0;
	buffer.read_uint16(item_count);
	for (int i = 0; i < item_count; ++i) {
		Item_Info *item = GAME_MANAGER->pop_item();
		item->deserialize(buffer);
		item_map[item->item_basic.index] = item;
	}

	long sec;
	buffer.read_int64(sec);
	last_wipe_time.set(sec, 0);

	buffer.read_int8((int8_t &)is_change_);

	return 0;
}

int Bag_Info::load(void) {
	return 0; //CACHED_INSTANCE->load_package(this->role_id, *this);
}

int Bag_Info::save(void) {
	if (is_change_)
		return 0; //CACHED_INSTANCE->save_package(*this);
	else
		return 0;
}

void Bag_Info::reset(void) {
	role_id = 0;
	capacity.pack_cap = PACK_INIT_CAPACITY;
	capacity.storage_cap = STORAGE_INIT_CAPACITY;
	money_info.reset();

	for (Item_Map::const_iterator it = item_map.begin(); it != item_map.end(); ++it) {
		GAME_MANAGER->push_item(it->second);
	}
	item_map.clear();
	money_lock_map.clear();
	item_lock_map.clear();
	last_wipe_time = Time_Value::zero;
	is_change_ = false;
}

Bag_Info &Bag_Info::operator=(Bag_Info &detail) {
	role_id = detail.role_id;
	capacity = detail.capacity;
	money_info = detail.money_info;

	Item_Info *item = 0;
	for (Item_Map::iterator it = detail.item_map.begin(); it != detail.item_map.end(); ++it) {
		item = GAME_MANAGER->pop_item();
		if (! item) {
			MSG_USER("item_pool().pop() return 0.");
			continue;
		}

		item->reset();
		(*item) = *(it->second);

		item_map[item->item_basic.index] = item;
	}

	last_wipe_time = detail.last_wipe_time;
	is_change_ = detail.is_change_;

	return *this;
}

void Bag_Info::erase(uint32_t index) {
	Item_Map::iterator it = item_map.find(index);
	if (it != item_map.end()) {
		GAME_MANAGER->push_item(it->second);
		item_map.erase(it);
	}
}

void Bag_Info::erase(Item_Map::iterator iter) {
	GAME_MANAGER->push_item(iter->second);
	item_map.erase(iter);
}

void Bag_Info::erase(Item_Map::iterator begin, Item_Map::iterator end) {
	for (Item_Map::iterator it = begin; it != end; ) {
		GAME_MANAGER->push_item(it->second);
		item_map.erase(it++);
	}
}

Player_Data::Player_Data(void) { reset(); }

int Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_int8(status);

	game_player_info.serialize(buffer);
	bag_info.serialize(buffer);
	return 0;
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_int8(status);

	game_player_info.deserialize(buffer);
	bag_info.deserialize(buffer);
	return 0;
}

int Player_Data::load() {
	game_player_info.role_id = role_id;
	game_player_info.load();
	bag_info.load();
	return 0;
}

int Player_Data::save(void) {
	game_player_info.save();
	bag_info.save();
	return 0;
}

void Player_Data::reset(void) {
	role_id = 0;
	status =	NULL_STATUS;

	game_player_info.reset();
	bag_info.reset();
}

int Player_DB_Cache::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_int32(gender);
	buffer.write_int32(career);
	buffer.write_int32(level);
	return 0;
}

int Player_DB_Cache::deserialize(Block_Buffer &buffer) {
	buffer.read_int64(role_id);
	buffer.read_string(account);
	buffer.read_string(role_name);
	buffer.read_int32(agent_num);
	buffer.read_int32(server_num);
	buffer.read_int32(gender);
	buffer.read_int32(career);
	buffer.read_int32(level);
	return 0;
}

void Player_DB_Cache::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	agent_num = 0;
	server_num = 0;
	gender = 0;
	career = 0;
	level = 0;
}
