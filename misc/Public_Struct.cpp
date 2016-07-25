/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Server_Config.h"
#include "Log.h"

void Server_Conf::init_server_conf(void) {
	const Json::Value &server_conf = SERVER_CONFIG->server_conf();
	Lib_Log::instance()->set_file_switcher(server_conf["lib_log_switcher"].asInt());
	Log::instance()->set_file_switcher(server_conf["server_log_switcher"].asInt());

	server_sleep_time = Time_Value(1, 0);					//1s
	receive_timeout = Time_Value(server_conf["receive_timeout"].asInt(), 0);	//900s
	server_send_interval = Time_Value(0, server_conf["server_send_interval"].asInt());				//100us
	connector_send_interval = Time_Value(0, server_conf["connector_send_interval"].asInt());	//100us

	server_ip = server_conf["server_ip"].asString();
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

Player_DB_Cache::Player_DB_Cache(void) {
	reset();
}

Player_DB_Cache::~Player_DB_Cache() {
}

void Player_DB_Cache::serialize(Block_Buffer &buffer) const {
	buffer.write_int64(role_id);
	buffer.write_string(account);
	buffer.write_string(role_name);
	buffer.write_int32(agent_num);
	buffer.write_int32(server_num);
	buffer.write_int32(level);
	buffer.write_int32(gender);
	buffer.write_int32(career);
}

int Player_DB_Cache::deserialize(Block_Buffer &buffer) {
	role_id = buffer.read_int64();
	account = buffer.read_string();
	role_name = buffer.read_string();
	agent_num = buffer.read_int32();
	server_num = buffer.read_int32();
	level = buffer.read_int32();
	gender = buffer.read_int32();
	career = buffer.read_int32();
	return 0;
}

void Player_DB_Cache::reset(void) {
	role_id = 0;
	account.clear();
	role_name.clear();
	agent_num = 0;
	server_num = 0;
	level = 0;
	gender = 0;
	career = 0;
}

Ip_Info::Ip_Info(void) {
	reset();
}

Ip_Info::~Ip_Info() {
}

void Ip_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(ip);
	buffer.write_int32(port);
}

int Ip_Info::deserialize(Block_Buffer &buffer) {
	ip = buffer.read_string();
	port = buffer.read_int32();
	return 0;
}

void Ip_Info::reset(void) {
	ip.clear();
	port = 0;
}

Login_Player_Info::Login_Player_Info(void) {
	reset();
}

Login_Player_Info::~Login_Player_Info() {
}

void Login_Player_Info::serialize(Block_Buffer &buffer) const {
	buffer.write_string(account);
	buffer.write_string(gate_ip);
	buffer.write_int32(gate_port);
	buffer.write_string(session);
	buffer.write_int64(session_tick);
}

int Login_Player_Info::deserialize(Block_Buffer &buffer) {
	account = buffer.read_string();
	gate_ip = buffer.read_string();
	gate_port = buffer.read_int32();
	session = buffer.read_string();
	session_tick = buffer.read_int64();
	return 0;
}

void Login_Player_Info::reset(void) {
	account.clear();
	gate_ip.clear();
	gate_port = 0;
	session.clear();
	session_tick = 0;
}
