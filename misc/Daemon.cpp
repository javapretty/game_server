/*
 * Daemon.cpp
 *
 *  Created on: 2016年3月26日
 *      Author: zhangyalei
 */

#include <Debug_Server.h>
#include <unistd.h>
#include "Lib_Log.h"
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
#include "Daemon.h"
#include "Daemon_Server.h"


Daemon::Daemon(int type) : type_(type) {
	watcher_ = new Epoll_Watcher;
}

Daemon::~Daemon(void) { }

void Daemon::loop(void) {
	Time_Value tv(3, 0);
	watcher_->add(this, Epoll_Watcher::EVENT_TIMEOUT, &tv);
	watcher_->loop();
}

int Daemon::handle_timeout(const Time_Value &tv) {
	pid_t ppid = getppid();
	if (ppid == 1) {
		watcher_->remove(this);
		switch (type_) {
		case Log::LOG_LOGIN_SERVER: {
			LOGIN_MANAGER->self_close_process();
			break;
		}
		case Log::LOG_GATE_SERVER: {
			GATE_MANAGER->self_close_process();
			break;
		}
		case Log::LOG_GAME_SERVER: {
			GAME_MANAGER->self_close_process();
			break;
		}
		case Log::LOG_MASTER_SERVER: {
			MASTER_MANAGER->self_close_process();
			break;
		}
		}
		exit(1);
	}
	return 0;
}

int Daemon::start_log_client(void) {
	int cid = -1;
	//Log_Connector
	LOG_CONNECTOR->set(server_conf_.server_ip, server_conf_.log_port, server_conf_.connect_send_interval);
	LOG_CONNECTOR->init();
	LOG_CONNECTOR->start();
	if ((cid = LOG_CONNECTOR->connect_server())< 2) {
		LOG_FATAL("log_connector fatal cid:%d,port:%d", cid, server_conf_.log_port);
	}
	LOG_CONNECTOR->thr_create();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_Log::Daemon_Log(void)
: Daemon(Log::LOG_LOG_SERVER)
{ }

Daemon_Log::~Daemon_Log(void) { }

Daemon_Log *Daemon_Log::instance_ = 0;

Daemon_Log *Daemon_Log::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Log;
	return instance_;
}

void Daemon_Log::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Log::start_server(void) {
	Log::instance()->set_log_type(Log::LOG_LOG_SERVER);
	server_conf_.init_server_conf();

	//Log_Server
	LOG_SERVER->set(server_conf_.log_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	LOG_SERVER->init();
	LOG_SERVER->start();
	LOG_SERVER->thr_create();
	LOG_DEBUG("log_server listen at port:%d", server_conf_.log_port);

	LOG_MANAGER->init();
	LOG_MANAGER->thr_create();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_Log::start_client(void) {
	start_log_client();
	Daemon::loop();
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_DB::Daemon_DB(void)
: Daemon(Log::LOG_DB_SERVER)
{ }

Daemon_DB::~Daemon_DB(void) { }

Daemon_DB *Daemon_DB::instance_ = 0;

Daemon_DB *Daemon_DB::instance(void) {
	if (! instance_)
		instance_ = new Daemon_DB;
	return instance_;
}

void Daemon_DB::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_DB::start_server(void) {
	/// start log client
	Log::instance()->set_log_type(Log::LOG_DB_SERVER);
	server_conf_.init_server_conf();

	//DB_Server
	DB_SERVER->set(server_conf_.db_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	DB_SERVER->init();
	DB_SERVER->start();
	DB_SERVER->thr_create();
	LOG_DEBUG("db_server listen at port:%d", server_conf_.db_port);

	DB_MANAGER->init();
	DB_MANAGER->start();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_DB::start_client(void) {
	start_log_client();
	Daemon::loop();
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_Login::Daemon_Login(void)
: Daemon(Log::LOG_LOGIN_SERVER)
{ }

Daemon_Login::~Daemon_Login(void) { }

Daemon_Login *Daemon_Login::instance_ = 0;

Daemon_Login *Daemon_Login::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Login;
	return instance_;
}

void Daemon_Login::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Login::start_server(void) {
	/// start log client
	Log::instance()->set_log_type(Log::LOG_LOGIN_SERVER);
	server_conf_.init_server_conf();

	/// Login Client Server
	LOGIN_CLIENT_SERVER->set(server_conf_.login_client_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.login_client_network_protocol);
	LOGIN_CLIENT_SERVER->init();
	LOGIN_CLIENT_SERVER->start();
	LOGIN_CLIENT_SERVER->thr_create();
	LOG_DEBUG("login_client_server listen at port:%d", server_conf_.login_client_port);

	/// Login Gate Server
	LOGIN_GATE_SERVER->set(server_conf_.login_gate_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	LOGIN_GATE_SERVER->init();
	LOGIN_GATE_SERVER->start();
	LOGIN_GATE_SERVER->thr_create();
	LOG_DEBUG("login_gate_server listen at port:%d", server_conf_.login_gate_port);

	LOGIN_MANAGER->init();
	LOGIN_MANAGER->thr_create();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_Login::start_client(void) {
	start_log_client();
	Daemon::loop();
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_Master::Daemon_Master(void)
: Daemon(Log::LOG_MASTER_SERVER)
{ }

Daemon_Master::~Daemon_Master(void) { }

Daemon_Master *Daemon_Master::instance_ = 0;

Daemon_Master *Daemon_Master::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Master;
	return instance_;
}

void Daemon_Master::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Master::start_server(void) {
	/// start log client
	Log::instance()->set_log_type(Log::LOG_MASTER_SERVER);
	server_conf_.init_server_conf();

	/// Master Gate Server
	MASTER_GATE_SERVER->set(server_conf_.master_gate_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	MASTER_GATE_SERVER->init();
	MASTER_GATE_SERVER->start();
	MASTER_GATE_SERVER->thr_create();
	LOG_DEBUG("master_gate_server listen at port:%d", server_conf_.master_gate_port);

	/// Master Game Server
	MASTER_GAME_SERVER->set(server_conf_.master_game_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	MASTER_GAME_SERVER->init();
	MASTER_GAME_SERVER->start();
	MASTER_GAME_SERVER->thr_create();
	LOG_DEBUG("master_game_server listen at port:%d", server_conf_.master_game_port);

	MASTER_MANAGER->init();
	MASTER_MANAGER->thr_create();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_Master::start_client(void) {
	start_log_client();
	
	int cid = -1;
	MASTER_DB_CONNECTOR->set(server_conf_.server_ip, server_conf_.db_port, server_conf_.connect_send_interval);
	MASTER_DB_CONNECTOR->init();
	MASTER_DB_CONNECTOR->start();
	if ((cid = MASTER_DB_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("master_db_connector fatal cid:%d,port:%d", cid, server_conf_.db_port);
	}
	MASTER_DB_CONNECTOR->thr_create();
	//加载PUBLIC数据
	MASTER_MANAGER->load_public_data();

	MASTER_V8_MANAGER->thr_create();			//master server v8 engine
	Daemon::loop();
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_Game::Daemon_Game(void)
: Daemon(Log::LOG_GAME_SERVER)
{ }

Daemon_Game::~Daemon_Game(void) { }

Daemon_Game *Daemon_Game::instance_ = 0;

Daemon_Game *Daemon_Game::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Game;
	return instance_;
}

void Daemon_Game::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Game::start_server(void) {
	/// start log client
	Log::instance()->set_log_type(Log::LOG_GAME_SERVER);
	server_conf_.init_server_conf();

	/// Game Gate Server
	GAME_GATE_SERVER->set(server_conf_.game_gate_port, server_conf_.receive_timeout, server_conf_.server_send_interval);
	GAME_GATE_SERVER->init();
	GAME_GATE_SERVER->start();
	GAME_GATE_SERVER->thr_create();
	LOG_DEBUG("game_gate_server listen at port:%d", server_conf_.game_gate_port);

	GAME_MANAGER->init();
	GAME_MANAGER->thr_create();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_Game::start_client(void) {
	start_log_client();

	int cid = -1;
	/// Game DB Connector
	GAME_DB_CONNECTOR->set(server_conf_.server_ip, server_conf_.db_port, server_conf_.connect_send_interval);
	GAME_DB_CONNECTOR->init();
	GAME_DB_CONNECTOR->start();
	if ((cid = GAME_DB_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("game_db_connector fatal cid:%d,port:%d", cid, server_conf_.db_port);
	}
	GAME_DB_CONNECTOR->thr_create();
	//加载DB缓存数据
	GAME_MANAGER->load_db_cache();

	/// Game Master Connector
	GAME_MASTER_CONNECTOR->set(server_conf_.server_ip, server_conf_.master_game_port, server_conf_.connect_send_interval);
	GAME_MASTER_CONNECTOR->init();
	GAME_MASTER_CONNECTOR->start();
	if ((cid = GAME_MASTER_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("game_master_connector fatal cid:%d,port:%d", cid, server_conf_.master_game_port);
	}
	GAME_MASTER_CONNECTOR->thr_create();

	GAME_V8_MANAGER->thr_create();				//game server v8 engine

	Daemon::loop();
}

///////////////////////////////////////////////////////////////////////////////////////////////
Daemon_Gate::Daemon_Gate(void)
: Daemon(Log::LOG_GATE_SERVER)
{ }

Daemon_Gate::~Daemon_Gate(void) { }

Daemon_Gate *Daemon_Gate::instance_ = 0;

Daemon_Gate *Daemon_Gate::instance(void) {
	if (! instance_)
		instance_ = new Daemon_Gate;
	return instance_;
}

void Daemon_Gate::destroy(void) {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void Daemon_Gate::start_server(void) {
	/// start log client
	Log::instance()->set_log_type(Log::LOG_GATE_SERVER);
	server_conf_.init_server_conf();

	/// Gate Client Server
	GATE_CLIENT_SERVER->set(server_conf_.gate_client_port, server_conf_.receive_timeout, server_conf_.server_send_interval, server_conf_.gate_client_network_protocol);
	GATE_CLIENT_SERVER->init();
	GATE_CLIENT_SERVER->start();
	GATE_CLIENT_SERVER->thr_create();
	LOG_DEBUG("gate_client_server listen at port:%d", server_conf_.gate_client_port);

	GATE_MANAGER->init();
	GATE_MANAGER->thr_create();

	//延迟让服务器启动
	Time_Value::sleep(server_conf_.server_sleep_time);
}

void Daemon_Gate::start_client(void) {
	start_log_client();

	int cid = -1;
	/// Gate Login Connector
	GATE_LOGIN_CONNECTOR->set(server_conf_.server_ip, server_conf_.login_gate_port, server_conf_.connect_send_interval);
	GATE_LOGIN_CONNECTOR->init();
	GATE_LOGIN_CONNECTOR->start();
	if ((cid = GATE_LOGIN_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("gate_login_connector fatal cid:%d,port:%d", cid, server_conf_.login_gate_port);
	}
	GATE_LOGIN_CONNECTOR->thr_create();

	/// Gate Game Connector
	GATE_GAME_CONNECTOR->set(server_conf_.server_ip, server_conf_.game_gate_port, server_conf_.connect_send_interval);
	GATE_GAME_CONNECTOR->init();
	GATE_GAME_CONNECTOR->start();
	if ((cid = GATE_GAME_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("gate_game_connector fatal cid:%d,port:%d", cid, server_conf_.game_gate_port);
	}
	GATE_GAME_CONNECTOR->thr_create();

	/// Gate Master Connector
	GATE_MASTER_CONNECTOR->set(server_conf_.server_ip, server_conf_.master_gate_port, server_conf_.connect_send_interval);
	GATE_MASTER_CONNECTOR->init();
	GATE_MASTER_CONNECTOR->start();
	if ((cid = GATE_MASTER_CONNECTOR->connect_server()) < 2) {
		LOG_FATAL("gate_master_connector fatal cid:%d,port:%d", cid, server_conf_.master_gate_port);
	}
	GATE_MASTER_CONNECTOR->thr_create();

	Daemon::loop();
}
