/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Debug_Server.h"
#include "Daemon_Server.h"
#include "Server_Config.h"
#include "Login_Manager.h"
#include "Login_Server.h"
#include "Login_Timer.h"
#include "Login_Client_Messager.h"
#include "Login_Inner_Messager.h"

Login_Manager::Login_Manager(void):
  mysql_conn_(NULL) {}

Login_Manager::~Login_Manager(void) {
	MYSQL_MANAGER->rel_mysql_conn(mysql_conn_);
}

Login_Manager *Login_Manager::instance_;

Login_Manager *Login_Manager::instance(void) {
	if (instance_ == 0)
		instance_ = new Login_Manager;
	return instance_;
}

int Login_Manager::init(int server_id) {
	init_data(server_id, "Login_Server");
	LOGIN_INNER_MESSAGER;
	LOGIN_CLIENT_MESSAGER;
	LOGIN_TIMER->thr_create();
	connect_mysql_db();
	return 0;
}

int Login_Manager::close_client(int gate_cid, int player_cid, int error_code) {
	if (Server_Manager::close_client(gate_cid, player_cid, error_code) < 0) return -1;

	if (error_code != 0) {
		//客户端主动断开连接不通知客户端，其他情况通知
		Block_Buffer buf;
		buf.make_server_message(ACTIVE_DISCONNECT, error_code);
		buf.finish_message();
		send_to_client(player_cid, buf);
	}
	return 0;
}

int Login_Manager::recycle_player(int gate_cid, int player_cid) {
	Server_Manager::recycle_player(gate_cid, player_cid);
	//断开客户端与服务器的通信层连接
	LOGIN_CLIENT_SERVER->receive().push_drop(player_cid);

	Login_Player *player = dynamic_cast<Login_Player*>(find_cid_player(player_cid));
	if (!player) {
		return -1;
	}

	player_cid_map_.erase(player->player_cid());
	player_role_id_map_.erase(player->role_id());
	player_account_map_.erase(player->account());
	player->reset();
	push_player(player);
	return 0;
}

int Login_Manager::free_cache(void) {
	Server_Manager::free_cache();
	player_pool_.shrink_all();
	LOGIN_CLIENT_SERVER->free_cache();
	LOGIN_GATE_SERVER->free_cache();
	return 0;
}

void Login_Manager::get_server_info(void) {
	login_gate_server_info_.reset();
	login_client_server_info_.reset();
	LOGIN_GATE_SERVER->get_server_info(login_gate_server_info_);
	LOGIN_CLIENT_SERVER->get_server_info(login_client_server_info_);
}

void Login_Manager::print_server_info(void) {
	Server_Manager::print_server_info();
	LOG_INFO("Login_Server player_pool_ free = %d, used = %d", player_pool_.free_obj_list_size(), player_pool_.used_obj_list_size());
}

int Login_Manager::send_to_client(int player_cid, Block_Buffer &buf) {
	if (player_cid < 2) {
		LOG_ERROR("player_cid = %d", player_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return LOGIN_CLIENT_SERVER->send_block(player_cid, buf);
}

int Login_Manager::send_to_gate(int gate_cid, Block_Buffer &buf){
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}
	add_send_bytes(buf.readable_bytes());
	return LOGIN_GATE_SERVER->send_block(gate_cid, buf);
}

int Login_Manager::process_list(void) {
	int32_t cid = 0;
	Block_Buffer *buf = 0;

	while (1) {
		bool all_empty = true;

		//掉线玩家列表
		if (! drop_cid_list_.empty()) {
			all_empty = false;
			cid = drop_cid_list_.pop_front();
			process_drop_cid(cid);
		}
		//client-->login
		if ((buf = login_client_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				LOGIN_CLIENT_MESSAGER->process_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			LOGIN_CLIENT_SERVER->push_block(cid, buf);
		}
		//gate-->login
		if ((buf = login_gate_data_list_.pop_front()) != 0) {
			all_empty = false;
			if (buf->is_legal()) {
				buf->peek_int32(cid);
				LOGIN_INNER_MESSAGER->process_gate_block(*buf);
			} else {
				LOG_ERROR("buf.read_index = %ld, buf.write_index = %ld", buf->get_read_idx(), buf->get_write_idx());
				buf->reset();
			}
			LOGIN_GATE_SERVER->push_block(cid, buf);
		}
		//定时器列表
		if (! tick_list_.empty()) {
			all_empty = false;
			tick_list_.pop_front();
			tick();
		}

		if (all_empty)
			Time_Value::sleep(Time_Value(0,100));
	}
	return 0;
}

void Login_Manager::process_drop_cid(int cid) {
	close_client(0, cid, 0);
}

int Login_Manager::connect_mysql_db() {
	const Json::Value &mysql_account = SERVER_CONFIG->server_misc()["mysql_account"];
	if (mysql_account == Json::Value::null) {
		LOG_FATAL("server_misc config error");
	}
	std::string ip(mysql_account["ip"].asString());
	int port = mysql_account["port"].asInt();
	std::string user(mysql_account["user"].asString());
	std::string password(mysql_account["password"].asString());
	std::string dbname(mysql_account["dbname"].asString());
	std::string dbpoolname(mysql_account["dbpoolname"].asString());

	MYSQL_MANAGER->init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_conn_ = MYSQL_MANAGER->get_mysql_conn(dbpoolname);
	return 0;
}

int Login_Manager::client_login(std::string& account, std::string& password){
	int ret = 0;
	try {
		std::string sql_query = "select account from account_info where account = \'" + account + "\' and password = \'" + password + "\'";
		sql::ResultSet* result = mysql_conn_->execute_query(sql_query.c_str());
		if (!result || result->rowsCount() <= 0) {
			sql_query = "insert into account_info values (\'" + account + "\', \'" +  password +"\')";
			ret = mysql_conn_->execute(sql_query.c_str());
		}
	} catch(sql::SQLException &e){
		int err_code = e.getErrorCode();
		LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = %s,%s, account = %s, password = %s",
				err_code, e.getSQLState().c_str(), e.what(), account.c_str(), password.c_str());
		ret = -1;
	}

	return ret;
}

void Login_Manager::get_gate_ip(std::string &account, std::string &ip, int &port) {
	int hash = elf_hash(account.c_str(), account.size());

	Server_List gate_list;
	int server_type = SERVER_CONFIG->server_misc()["server_type"].asInt();
	if (server_type == MULTI_PROCESS) {
		gate_list = DAEMON_SERVER->server_conf().gate_list;
	} else if (server_type == MULTI_THREAD) {
		gate_list = DEBUG_SERVER->server_conf().gate_list;
	}

	int index = hash % (gate_list.size());
	ip = gate_list[index].server_ip;
	port = gate_list[index].server_port;
}
