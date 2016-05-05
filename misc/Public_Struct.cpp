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

Player_Data_Ctl::Player_Data_Ctl(Player_Data *data) {
	player_data = data;
	reset();
}

int Player_Data_Ctl::serialize(Block_Buffer &buffer) const {
	player_data->serialize(buffer);
	return 0;
}

int Player_Data_Ctl::deserialize(Block_Buffer &buffer) {
	return player_data->deserialize(buffer);
}

int Player_Data_Ctl::load(int64_t role_id) {
	CACHED_INSTANCE->load_player_info(role_id, player_data->player_info);
	CACHED_INSTANCE->load_hero_info(role_id, player_data->hero_info);
	CACHED_INSTANCE->load_bag_info(role_id, player_data->bag_info);
	CACHED_INSTANCE->load_mail_info(role_id, player_data->mail_info);
	CACHED_INSTANCE->load_shop_info(role_id, player_data->shop_info);
	return 0;
}

int Player_Data_Ctl::save(void) {
	for (std::vector<int>::iterator it = player_data->change_module.begin(); it != player_data->change_module.end(); ++it) {
		switch(*it) {
		case PLAYER_CHANGE:
			CACHED_INSTANCE->save_player_info(player_data->player_info.role_id, player_data->player_info);
			break;
		case HERO_CHANGE:
			CACHED_INSTANCE->save_hero_info(player_data->player_info.role_id, player_data->hero_info);
			break;
		case BAG_CHANGE:
			CACHED_INSTANCE->save_bag_info(player_data->player_info.role_id, player_data->bag_info);
			break;
		case MAIL_CHANGE:
			CACHED_INSTANCE->save_mail_info(player_data->player_info.role_id, player_data->mail_info);
			break;
		case SHOP_CHANGE:
			CACHED_INSTANCE->save_shop_info(player_data->player_info.role_id, player_data->shop_info);
			break;
		}
	}
	return 0;
}

void Player_Data_Ctl::reset(void) {
	player_data->reset();
}

void Player_Data_Ctl::set_all_change(bool is_change) {
	if (is_change) {
		for (int i = PLAYER_CHANGE; i < CHANGE_END; ++i) {
			player_data->change_module.push_back(i);
		}
	} else {
		player_data->change_module.clear();
	}
}
