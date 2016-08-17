/*
 * Gate_Connector.cpp
 *
 *  Created on: 2016年1月13日
 *      Author: zhangyalei
 */

#include "Gate_Connector.h"
#include "Gate_Manager.h"

Gate_Game_Connector::Gate_Game_Connector(void){ }

Gate_Game_Connector::~Gate_Game_Connector(void) { }

void Gate_Game_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_game_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

Gate_Game_Connector_Manager::Gate_Game_Connector_Manager(void) { }

Gate_Game_Connector_Manager::~Gate_Game_Connector_Manager(void) { }

Gate_Game_Connector_Manager *Gate_Game_Connector_Manager::instance_ = 0;

Gate_Game_Connector_Manager *Gate_Game_Connector_Manager::instance(void) {
	if (! instance_)
		instance_ = new Gate_Game_Connector_Manager;
	return instance_;
}

void Gate_Game_Connector_Manager::start_new_connector(int id, std::string ip, int port, Time_Value &send_interval) {
	Gate_Game_Connector *connector = new Gate_Game_Connector();
	connector->set(ip, port, send_interval);
	connector->init();
	connector->start();
	int cid = 0;
	if ((cid = connector->connect_server()) < 2) {
		LOG_FATAL("gate_game_connector fatal cid:%d,port:%d", cid, port);
	}
	connector->thr_create();
	game_map_[cid] = connector;
	GATE_MANAGER->add_new_game(cid, id);
}

void Gate_Game_Connector_Manager::free_cache() {
	for(GATE_GAME_MAP::iterator iter = game_map_.begin();
			iter != game_map_.end(); iter++){
		iter->second->free_cache();
	}
}

void Gate_Game_Connector_Manager::push_block(int cid, Block_Buffer *buffer) {
	Gate_Game_Connector* connector;
	GATE_GAME_MAP::iterator iter = game_map_.find(cid);
	if(iter != game_map_.end())
		connector = iter->second;
	connector->push_block(cid, buffer);
}

int Gate_Game_Connector_Manager::send_block(int cid, Block_Buffer &buf) {
	Gate_Game_Connector* connector;
	GATE_GAME_MAP::iterator iter = game_map_.find(cid);
	if(iter != game_map_.end())
		connector = iter->second;
	return connector->send_block(cid, buf);
}

///////////////////////////////////////////////////////////////////

Gate_Login_Connector::Gate_Login_Connector(void) { }

Gate_Login_Connector::~Gate_Login_Connector(void) { }

Gate_Login_Connector *Gate_Login_Connector::instance_ = 0;

Gate_Login_Connector *Gate_Login_Connector::instance(void) {
	if (! instance_)
		instance_ = new Gate_Login_Connector;
	return instance_;
}

void Gate_Login_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_login_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////

Gate_Master_Connector::Gate_Master_Connector(void) { }

Gate_Master_Connector::~Gate_Master_Connector(void) { }

Gate_Master_Connector *Gate_Master_Connector::instance_ = 0;

Gate_Master_Connector *Gate_Master_Connector::instance(void) {
	if (! instance_)
		instance_ = new Gate_Master_Connector;
	return instance_;
}

void Gate_Master_Connector::process_list(void) {
	Block_Buffer *buf = 0;
	while (1) {
		if (!block_list_.empty()) {
			buf = block_list_.pop_front();
			GATE_MANAGER->push_gate_master_data(buf);
		} else {
			//没有数据时候延迟
			Time_Value::sleep(Time_Value(0,100));
		}
	}
}
