/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "DB_Operator.h"

const Time_Value Recycle_Tick::recycle_time_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::tick_interval_ = Time_Value(2, 0);
const Time_Value Recycle_Tick::session_interval_ = Time_Value(30, 0);

Player_Data::Player_Data(void) { reset(); }

int Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(status);

	buffer.write_uint16(change_set.size());
	for(boost::unordered_set<int>::const_iterator it = change_set.begin();
			it != change_set.end(); ++it) {
		buffer.write_int32(*it);
	}
	player_info.serialize(buffer);
	hero_info.serialize(buffer);
	bag_info.serialize(buffer);
	mail_info.serialize(buffer);
	return 0;
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	status = buffer.read_int8();

	uint16_t size = buffer.read_uint16();
	int change_id = 0;
	for (int16_t i = 0; i < size; ++i) {
		change_id = buffer.read_int32();
		change_set.insert(change_id);
	}
	player_info.deserialize(buffer);
	hero_info.deserialize(buffer);
	bag_info.deserialize(buffer);
	mail_info.deserialize(buffer);
	return 0;
}

int Player_Data::load(int64_t role_id) {
	CACHED_INSTANCE->load_player_info(role_id, player_info);
	CACHED_INSTANCE->load_hero_info(role_id, hero_info);
	CACHED_INSTANCE->load_bag_info(role_id, bag_info);
	CACHED_INSTANCE->load_mail_info(role_id, mail_info);
	return 0;
}

int Player_Data::save(void) {
	for (boost::unordered_set<int>::iterator it = change_set.begin(); it != change_set.end(); ++it) {
		switch(*it) {
		case PLAYER_CHANGE:
			CACHED_INSTANCE->save_player_info(player_info.role_id, player_info);
			break;
		case HERO_CHANGE:
			CACHED_INSTANCE->save_hero_info(player_info.role_id, hero_info);
			break;
		case BAG_CHANGE:
			CACHED_INSTANCE->save_bag_info(player_info.role_id, bag_info);
			break;
		case MAIL_CHANGE:
			CACHED_INSTANCE->save_mail_info(player_info.role_id, mail_info);
			break;
		}
	}
	return 0;
}

void Player_Data::reset(void) {
	status =	NULL_STATUS;
	change_set.clear();

	player_info.reset();
	hero_info.reset();
	bag_info.reset();
	mail_info.reset();
}

void Player_Data::set_all_change(bool is_change) {
	if (is_change) {
		for (int i = CHANGE_FIRST + 1; i < CHANGE_END; ++i) {
			change_set.insert(i);
		}
	} else {
		change_set.clear();
	}
}

void Player_Data::set_change(int change_id) {
	change_set.insert(change_id);
}
