/*
 * Debug_Server.cpp
 *
 *  Created on: Dec 18, 2015
 *      Author: zhangyalei
 */

#include <unistd.h>
#include "Epoll_Watcher.h"
#include "Server_Config.h"
#include "DB_Server.h"
#include "DB_Manager.h"
#include "Log_Connector.h"
#include "Log_Server.h"
#include "Log_Manager.h"
#include "Login_Server.h"
#include "Login_Manager.h"
#include "Master_Server.h"
#include "Master_Manager.h"
#include "Master_Connector.h"
#include "Game_Server.h"
#include "Game_Connector.h"
#include "Game_Manager.h"
#include "Gate_Server.h"
#include "Gate_Connector.h"
#include "Gate_Manager.h"
#include "V8_Manager.h"
#include "Debug_Server.h"

Debug_Server::Debug_Server(void)
: wait_watcher_(0)
{ }

Debug_Server::~Debug_Server(void) { }

Debug_Server *Debug_Server::instance_;

Debug_Server *Debug_Server::instance(void) {
    if (! instance_)
        instance_ = new Debug_Server();
    return instance_;
}

void Debug_Server::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

int Debug_Server::init(int argc, char *argv[]) {
	wait_watcher_ = new Epoll_Watcher();
	Time_Value tv(3, 0);
	wait_watcher_->add(this, EVENT_TIMEOUT, &tv);

	server_conf_.init_server_conf();
	const Json::Value server_misc = SERVER_CONFIG->server_misc();
	Lib_Log::instance()->set_file_switcher(server_misc["lib_log_switcher"].asInt());
	Log::instance()->set_file_switcher(server_misc["server_log_switcher"].asInt());
	return 0;
}

int Debug_Server::start(int argc, char *argv[]) {
	/////////////////////start server///////////////////////
	//Log_Server
	LOG_SERVER->set(server_conf_.log_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.log_server.network_protocol);
	LOG_SERVER->init();
	LOG_SERVER->start();
	LOG_SERVER->thr_create();
	LOG_INFO("log_server server_id:%d listen at port:%d", server_conf_.log_server.server_id, server_conf_.log_server.server_port);

	LOG_MANAGER->init(server_conf_.log_server.server_id);
	LOG_MANAGER->thr_create();

	//DB_Server
	DB_SERVER->set(server_conf_.db_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.log_server.network_protocol);
	DB_SERVER->init();
	DB_SERVER->start();
	DB_SERVER->thr_create();
	LOG_INFO("db_server server_id:%d listen at port:%d", server_conf_.db_server.server_id, server_conf_.db_server.server_port);

	DB_MANAGER->init(server_conf_.db_server.server_id);
	DB_MANAGER->start();

	/// Login Client Server
	LOGIN_CLIENT_SERVER->set(server_conf_.login_client_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.login_client_server.network_protocol);
	LOGIN_CLIENT_SERVER->init();
	LOGIN_CLIENT_SERVER->start();
	LOGIN_CLIENT_SERVER->thr_create();
	LOG_INFO("login_client_server server_id:%d listen at port:%d", server_conf_.login_client_server.server_id, server_conf_.login_client_server.server_port);

	/// Login Gate Server
	LOGIN_GATE_SERVER->set(server_conf_.login_gate_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.login_gate_server.network_protocol);
	LOGIN_GATE_SERVER->init();
	LOGIN_GATE_SERVER->start();
	LOGIN_GATE_SERVER->thr_create();
	LOG_INFO("login_gate_server server_id:%d listen at port:%d", server_conf_.login_gate_server.server_id, server_conf_.login_gate_server.server_port);

	LOGIN_MANAGER->init(server_conf_.login_client_server.server_id);
	LOGIN_MANAGER->thr_create();

	/// Master Gate Server
	MASTER_GATE_SERVER->set(server_conf_.master_gate_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.master_gate_server.network_protocol);
	MASTER_GATE_SERVER->init();
	MASTER_GATE_SERVER->start();
	MASTER_GATE_SERVER->thr_create();
	LOG_INFO("master_gate_server server_id:%d listen at port:%d", server_conf_.master_gate_server.server_id, server_conf_.master_gate_server.server_port);

	/// Master Game Server
	MASTER_GAME_SERVER->set(server_conf_.master_game_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.master_game_server.network_protocol);
	MASTER_GAME_SERVER->init();
	MASTER_GAME_SERVER->start();
	MASTER_GAME_SERVER->thr_create();
	LOG_INFO("master_game_server server_id:%d listen at port:%d", server_conf_.master_game_server.server_id, server_conf_.master_game_server.server_port);

	/// Master Http Server
	MASTER_HTTP_SERVER->set(server_conf_.master_http_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.master_http_server.network_protocol);
	MASTER_HTTP_SERVER->init();
	MASTER_HTTP_SERVER->start();
	MASTER_HTTP_SERVER->thr_create();
	LOG_INFO("master_http_server server_id:%d listen at port:%d", server_conf_.master_http_server.server_id, server_conf_.master_http_server.server_port);

	MASTER_MANAGER->init(server_conf_.master_gate_server.server_id);
	MASTER_MANAGER->thr_create();

	/// Game Gate Server
	Server_Detail game_gate_server = server_conf_.game_list[0];
	GAME_GATE_SERVER->set(game_gate_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, game_gate_server.network_protocol);
	GAME_GATE_SERVER->init();
	GAME_GATE_SERVER->start();
	GAME_GATE_SERVER->thr_create();
	LOG_INFO("game_gate_server server_id:%d listen at port:%d", game_gate_server.server_id, game_gate_server.server_port);

	GAME_MANAGER->init(game_gate_server.server_id);
	GAME_MANAGER->thr_create();

	/// Gate Client Server
	Server_Detail gate_client_server = server_conf_.gate_list[0];
	GATE_CLIENT_SERVER->set(gate_client_server.server_port, server_conf_.receive_timeout, server_conf_.server_send_interval, gate_client_server.network_protocol);
	GATE_CLIENT_SERVER->init();
	GATE_CLIENT_SERVER->start();
	GATE_CLIENT_SERVER->thr_create();
	LOG_INFO("gate_client_server server_id:%d listen at port:%d", gate_client_server.server_id, gate_client_server.server_port);

	GATE_MANAGER->init(gate_client_server.server_id);
	GATE_MANAGER->thr_create();
	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);

	/////////////////////start connect//////////////////////////////
	int cid = -1;
	//Log_Connector
	LOG_CONNECTOR->set(server_conf_.log_server.server_ip, server_conf_.log_server.server_port, server_conf_.connector_send_interval);
	LOG_CONNECTOR->init();
	LOG_CONNECTOR->start();
	if ((cid = LOG_CONNECTOR->connect_server())< 2) {
		LOG_FATAL("log_connector fatal cid:%d,port:%d", cid, server_conf_.log_server.server_port);
	}
	LOG_CONNECTOR->thr_create();

	/// Master DB Connector
	MASTER_DB_CONNECTOR->set(server_conf_.db_server.server_ip, server_conf_.db_server.server_port, server_conf_.connector_send_interval);
	MASTER_DB_CONNECTOR->init();
	MASTER_DB_CONNECTOR->start();
	if ((cid = MASTER_DB_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("master_db_connector fatal cid:%d,port:%d", cid, server_conf_.db_server.server_port);
	}
	MASTER_DB_CONNECTOR->thr_create();
	MASTER_MANAGER->load_master_db_data();

	/// Game DB Connector
	GAME_DB_CONNECTOR->set(server_conf_.db_server.server_ip, server_conf_.db_server.server_port, server_conf_.connector_send_interval);
	GAME_DB_CONNECTOR->init();
	GAME_DB_CONNECTOR->start();
	if ((cid = GAME_DB_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("game_db_connector fatal cid:%d,port:%d", cid, server_conf_.db_server.server_port);
	}
	GAME_DB_CONNECTOR->thr_create();

	/// Game Master Connector
	GAME_MASTER_CONNECTOR->set(server_conf_.master_game_server.server_ip, server_conf_.master_game_server.server_port, server_conf_.connector_send_interval);
	GAME_MASTER_CONNECTOR->init();
	GAME_MASTER_CONNECTOR->start();
	if ((cid = GAME_MASTER_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("game_master_connector fatal cid:%d,port:%d", cid, server_conf_.master_game_server.server_port);
	}
	GAME_MASTER_CONNECTOR->thr_create();

	/// Gate Login Connector
	GATE_LOGIN_CONNECTOR->set(server_conf_.login_gate_server.server_ip, server_conf_.login_gate_server.server_port, server_conf_.connector_send_interval);
	GATE_LOGIN_CONNECTOR->init();
	GATE_LOGIN_CONNECTOR->start();
	if ((cid = GATE_LOGIN_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("gate_login_connector fatal cid:%d,port:%d", cid, server_conf_.login_gate_server.server_port);
	}
	GATE_LOGIN_CONNECTOR->thr_create();

	/// Gate Game Connector
	GATE_MANAGER->new_game_connector(game_gate_server.server_id, game_gate_server.server_ip, game_gate_server.server_port, server_conf_.connector_send_interval);

	/// Gate Master Connector
	GATE_MASTER_CONNECTOR->set(server_conf_.master_gate_server.server_ip, server_conf_.master_gate_server.server_port, server_conf_.connector_send_interval);
	GATE_MASTER_CONNECTOR->init();
	GATE_MASTER_CONNECTOR->start();
	if ((cid = GATE_MASTER_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("gate_master_connector fatal cid:%d,port:%d", cid, server_conf_.master_gate_server.server_port);
	}
	GATE_MASTER_CONNECTOR->thr_create();

	GAME_V8_MANAGER->thr_create();				//game server v8 engine
	MASTER_V8_MANAGER->thr_create();			//master server v8 engine

	return wait_watcher_->loop();
}
