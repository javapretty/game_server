/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "DB_Operator.h"
#include "Server_Config.h"
#include "Log.h"

void Server_Conf::init_server_conf(void) {
	const Json::Value &server_conf = SERVER_CONFIG->server_conf();
	Lib_Log::instance()->set_file_switcher(server_conf["lib_log_switcher"].asInt());
	Log::instance()->set_file_switcher(server_conf["server_log_switcher"].asInt());

	server_sleep_time = Time_Value(1, 0);					//1s
	receive_timeout = Time_Value(server_conf["recv_timeout"].asInt(), 0);
	server_send_interval = Time_Value(0, 100);		//100us
	connect_send_interval = Time_Value(0, 100);		//100us

	server_ip = "127.0.0.1";
	log_port = server_conf["log_server"]["port"].asInt();
	db_port = server_conf["db_server"]["port"].asInt();
	login_client_network_protocol = server_conf["login_server"]["client_network_protocol"].asInt();
	login_client_port = server_conf["login_server"]["client_port"].asInt();
	login_gate_port = server_conf["login_server"]["gate_port"].asInt();
	master_gate_port = server_conf["master_server"]["gate_port"].asInt();
	master_game_port = server_conf["master_server"]["game_port"].asInt();
	game_gate_port = server_conf["game_server"]["gate_port"].asInt();
	gate_client_network_protocol = server_conf["gate_server"]["client_network_protocol"].asInt();
	gate_client_port = server_conf["gate_server"]["client_port"].asInt();
}

Player_Data::Player_Data(void) { reset(); }

int Player_Data::serialize(Block_Buffer &buffer) const {
	buffer.write_int8(status);

	buffer.write_uint16(change_module.size());
	for(std::vector<int>::const_iterator it = change_module.begin();
			it != change_module.end(); ++it) {
		buffer.write_int32(*it);
	}
	player_info.serialize(buffer);
	hero_info.serialize(buffer);
	bag_info.serialize(buffer);
	mail_info.serialize(buffer);
	shop_info.serialize(buffer);
	return 0;
}

int Player_Data::deserialize(Block_Buffer &buffer) {
	status = buffer.read_int8();

	uint16_t size = buffer.read_uint16();
	int change_id = 0;
	for (int16_t i = 0; i < size; ++i) {
		change_id = buffer.read_int32();
		change_module.push_back(change_id);
	}
	player_info.deserialize(buffer);
	hero_info.deserialize(buffer);
	bag_info.deserialize(buffer);
	mail_info.deserialize(buffer);
	shop_info.deserialize(buffer);
	return 0;
}

int Player_Data::load(int64_t role_id) {
	CACHED_INSTANCE->load_player_info(role_id, player_info);
	CACHED_INSTANCE->load_hero_info(role_id, hero_info);
	CACHED_INSTANCE->load_bag_info(role_id, bag_info);
	CACHED_INSTANCE->load_mail_info(role_id, mail_info);
	CACHED_INSTANCE->load_shop_info(role_id, shop_info);
	return 0;
}

int Player_Data::save(void) {
	for (std::vector<int>::iterator it = change_module.begin(); it != change_module.end(); ++it) {
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
		case SHOP_CHANGE:
			CACHED_INSTANCE->save_shop_info(player_info.role_id, shop_info);
			break;
		}
	}
	return 0;
}

void Player_Data::reset(void) {
	status =	NULL_STATUS;
	change_module.clear();

	player_info.reset();
	hero_info.reset();
	bag_info.reset();
	mail_info.reset();
	shop_info.reset();
}

void Player_Data::set_all_change(bool is_change) {
	if (is_change) {
		for (int i = PLAYER_CHANGE; i < CHANGE_END; ++i) {
			change_module.push_back(i);
		}
	} else {
		change_module.clear();
	}
}
