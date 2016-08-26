/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#include "Common_Func.h"
#include "Debug_Server.h"
#include "Daemon_Server.h"
#include "Server_Config.h"
#include "Login_Manager.h"
#include "Login_Server.h"
#include "Login_Timer.h"
#include "Login_Client_Messager.h"
#include "Login_Inner_Messager.h"

Login_Manager::Login_Manager(void):
  player_cid_map_(get_hash_table_size(12000)) ,
  player_account_map_(get_hash_table_size(12000)),
	server_status_(STATUS_NORMAL),
	msg_count_onoff_(true),
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

int Login_Manager::init(void) {
	tick_time_ = Time_Value::gettimeofday();

	LOGIN_INNER_MESSAGER;					/// 内部消息处理
	LOGIN_CLIENT_MESSAGER;					/// 外部消息处理
	LOGIN_TIMER->thr_create();
	connect_mysql_db();
	return 0;
}

void Login_Manager::run_handler(void) {
	process_list();
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

int Login_Manager::bind_account_login_player(std::string& account, Login_Player *player) {
	if (! player_account_map_.insert(std::make_pair(account, player)).second) {
		LOG_ERROR("insert account login player fail, account:%s, role_id:%ld", account.c_str());
	}
	return 0;
}

int Login_Manager::unbind_account_login_player(std::string& account) {
	player_account_map_.erase(account);
	return 0;
}

Login_Player *Login_Manager::find_account_login_player(std::string& account) {
	Login_Player_Account_Map::iterator it = player_account_map_.find(account);
	if (it != player_account_map_.end())
		return it->second;
	else
		return 0;
}

int Login_Manager::bind_cid_login_player(int cid, Login_Player *player) {
	if (! player_cid_map_.insert(std::make_pair(cid, player)).second) {
		LOG_ERROR("insert cid login player fail, cid:%d", cid);
	}
	return 0;
}

int Login_Manager::unbind_cid_login_player(int cid) {
	player_cid_map_.erase(cid);
	return 0;
}

Login_Player *Login_Manager::find_cid_login_player(int cid) {
	Login_Player_Cid_Map::iterator it = player_cid_map_.find(cid);
	if (it != player_cid_map_.end())
		return it->second;
	else
		return 0;
}

int Login_Manager::unbind_login_player(Login_Player &player) {
	player_account_map_.erase(player.login_player_info().account);
	player_cid_map_.erase(player.get_cid());
	return 0;
}

int Login_Manager::send_to_client(int player_cid, Block_Buffer &buf) {
	if (player_cid < 2) {
		LOG_ERROR("player_cid = %d", player_cid);
		return -1;
	}
	return LOGIN_CLIENT_SERVER->send_block(player_cid, buf);
}

int Login_Manager::send_to_gate(int gate_cid, Block_Buffer &buf){
	if (gate_cid < 2) {
		LOG_ERROR("gate_cid = %d", gate_cid);
		return -1;
	}

	return LOGIN_GATE_SERVER->send_block(gate_cid, buf);
}

int Login_Manager::close_client(int player_cid) {
	if (player_cid >= 2) {
		Close_Info info(player_cid, tick_time());
		close_list_.push_back(info);
	} else {
		LOG_ERROR("close_client, player_cid < 2");
	}
	return 0;
}

int Login_Manager::self_close_process(void) {
	server_status_ = STATUS_CLOSING;

	Block_Buffer buf;
	buf.make_server_message(ACTIVE_DISCONNECT, ERROR_DISCONNECT_SELF);
	buf.finish_message();
	for (Login_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		LOGIN_MANAGER->send_to_client(iter->first, buf);
	}
	//等待服务器通知客户端完毕
	sleep(2);

	//关闭客户端连接
	for (Login_Player_Cid_Map::iterator iter = player_cid_map_.begin(); iter != player_cid_map_.end(); ++iter) {
		iter->second->link_close();
	}

	int i = 0;
	while (++i < 60) {
		sleep(1);
		LOG_INFO("login server has user:%d", player_cid_map_.size());
		if (player_cid_map_.size() == 0)
			break;
	}
	return 0;
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
	Login_Player *player = LOGIN_MANAGER->find_cid_login_player(cid);
	if (player) {
		player->link_close();
	}
}

int Login_Manager::tick(void) {
	Time_Value now(Time_Value::gettimeofday());
	tick_time_ = now;

	close_list_tick(now);
	player_tick(now);
	server_info_tick(now);
	object_pool_tick(now);
	//LOG->show_msg_time(now);
	return 0;
}

int Login_Manager::close_list_tick(Time_Value &now) {
	Close_Info info;
	while (! close_list_.empty()) {
		info = close_list_.front();
		if (now - info.timestamp > Time_Value(2, 0)) {
			close_list_.pop_front();
			LOGIN_CLIENT_SERVER->receive().push_drop(info.cid);
		} else {
			break;
		}
	}
	return 0;
}

int Login_Manager::server_info_tick(Time_Value &now) {
	if (now - tick_info_.server_info_last_tick < tick_info_.server_info_interval_tick)
		return 0;

	tick_info_.server_info_last_tick = now;

	login_gate_server_info_.reset();
	login_client_server_info_.reset();
	LOGIN_GATE_SERVER->get_server_info(login_gate_server_info_);
	LOGIN_CLIENT_SERVER->get_server_info(login_client_server_info_);

	return 0;
}

int Login_Manager::player_tick(Time_Value &now) {
	if (now - tick_info_.player_last_tick < tick_info_.player_interval_tick)
		return 0;
	tick_info_.player_last_tick = now;
	Login_Player_Account_Map t_accouont_map(player_account_map_); /// 因为Login_Player::time_up()里有改变player_account_map_的操作, 直接在其上使用迭代器导致迭代器失效core
	for (Login_Player_Account_Map::iterator it = t_accouont_map.begin(); it != t_accouont_map.end(); ++it) {
		if (it->second){
			if (now.sec() - it->second->login_player_info().session_tick > 10) {
				LOG_INFO("client drop from login, cid:%d, account:%s", it->second->get_cid(), it->second->login_player_info().account.c_str());
				LOGIN_CLIENT_SERVER->receive().push_drop(it->second->get_cid());	//断开客户端与login的连接
				it->second->link_close();
			}
			it->second->tick(now);
		}
	}

	return 0;
}

void Login_Manager::object_pool_tick(Time_Value &now) {
	if (now - tick_info_.object_pool_last_tick < tick_info_.object_pool_interval_tick)
		return;
	tick_info_.object_pool_last_tick = now;
	object_pool_size();
}

void Login_Manager::get_server_info(Block_Buffer &buf) {
	login_client_server_info_.serialize(buf);
	login_gate_server_info_.serialize(buf);
}

void Login_Manager::object_pool_size(void) {
	LOG_INFO("login block_pool_ free = %d, used = %d", block_pool_.free_obj_list_size(), block_pool_.used_obj_list_size());
}

void Login_Manager::free_cache(void) {
	LOGIN_CLIENT_SERVER->free_cache();
	LOGIN_GATE_SERVER->free_cache();

	block_pool_.shrink_all();
}

void Login_Manager::print_msg_count(void) {
	std::stringstream stream;
	for (Msg_Count_Map::iterator it = inner_msg_count_map_.begin(); it != inner_msg_count_map_.end(); ++it) {
		stream << (it->first) << "\t" << (it->second) << std::endl;
	}
	LOG_INFO("inner_msg_count_map_.size = %d\n%s\n", inner_msg_count_map_.size(), stream.str().c_str());
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
