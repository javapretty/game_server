/*
 * Mysql_Conn.cpp
 *
 *  Created on: Jan 4,2016
 *      Author: zhangyalei
 */

#include <sstream>
#include "Mysql_Conn.h"
#include "Lib_Log.h"

Mysql_Manager* Mysql_Manager::instance_ = NULL;

Mysql_Conn::Mysql_Conn(Mysql_Pool* mysql_pool) {
	mysql_pool_ = mysql_pool;
	conn_ = NULL;
	stmt_ = NULL;
}

Mysql_Conn::~Mysql_Conn() {
	if (conn_) {
		delete conn_;
		conn_ = NULL;
	}

	if (stmt_) {
		delete stmt_;
		stmt_ = NULL;
	}
}

int Mysql_Conn::init() {
	std::stringstream stream;
	stream << mysql_pool_->get_server_port();
	std::string url = std::string("tcp://") + mysql_pool_->get_server_ip() + ":" + stream.str();
	try {
		conn_ = mysql_pool_->get_driver()->connect(url.c_str(), mysql_pool_->get_username(), mysql_pool_->get_passwrod());
		conn_->setSchema(mysql_pool_->get_db_name());
		if (conn_->isClosed()) {
			LIB_LOG_ERROR("connect mysql error, url = [%s], user = [%s], password = [%s]", url.c_str(), mysql_pool_->get_username().c_str(), mysql_pool_->get_passwrod().c_str());
			return 1;
		}

		stmt_ = conn_->createStatement();
	}
	catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LIB_LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return 2;
	}

	return 0;
}

sql::PreparedStatement* Mysql_Conn::create_pstmt(const char* str_sql) {
	sql::PreparedStatement* pstmt = NULL;

	try {
		pstmt = conn_->prepareStatement(str_sql);
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LIB_LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return NULL;
	}
	return pstmt;
}

std::string& Mysql_Conn::get_pool_name() {
	return mysql_pool_->get_pool_name();
}

sql::ResultSet* Mysql_Conn::execute_query(const char* str_sql) {
	sql::ResultSet *res = NULL;
	try {
		res = stmt_->executeQuery(str_sql);
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LIB_LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return NULL;
	}
	return res;
}

int Mysql_Conn::execute_update(const char* str_sql) {
	int ret = 0;
	try {
		ret = stmt_->executeUpdate(str_sql);
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LIB_LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return -1;
	}
	return ret;
}

int Mysql_Conn::execute(const char* str_sql) {
	int ret = 0;
	try {
		ret = stmt_->execute(str_sql);
	} catch (sql::SQLException &e) {
		int err_code = e.getErrorCode();
		LIB_LOG_ERROR("SQLException, MySQL Error Code = %d, SQLState = [%s], [%s]", err_code, e.getSQLState().c_str(), e.what());
		return -1;
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
Mysql_Pool::Mysql_Pool(std::string& pool_name,  std::string& server_ip, uint32_t server_port,
		std::string& username, std::string& password,  std::string& db_name, int32_t max_conn_cnt) {
	pool_name_ = pool_name;
	server_ip_ = server_ip;
	server_port_ = server_port;
	username_ = username;
	password_ = password;
	db_name_ = db_name;
	max_conn_cnt_ = max_conn_cnt;
	cur_conn_cnt_ = 2;

	try {
		driver_ = get_driver_instance();
	} catch (sql::SQLException&e) {
		LIB_LOG_ERROR("mysql驱动连接出错;\n");
	} catch (std::runtime_error&e) {
		LIB_LOG_ERROR("mysql运行时错误\n");
	}
}

Mysql_Pool::~Mysql_Pool() {
	for (std::list<Mysql_Conn*>::iterator iter = free_list_.begin(); iter != free_list_.end(); iter++) {
		Mysql_Conn* mysql_conn = *iter;
		delete mysql_conn;
	}

	free_list_.clear();
}

int Mysql_Pool::init() {
	for (int i = 0; i < cur_conn_cnt_; i++) {
		Mysql_Conn* mysql_conn = new Mysql_Conn(this);
		int ret = mysql_conn->init();
		if (ret) {
			delete mysql_conn;
			return ret;
		}
		free_list_.push_back(mysql_conn);
	}

	LIB_LOG_INFO("db pool: %s, size: %d", pool_name_.c_str(), (int)free_list_.size());
	return 0;
}

Mysql_Conn* Mysql_Pool::get_mysql_conn() {
	thread_notify_.lock();

	while (free_list_.empty()) {
		if (cur_conn_cnt_ >= max_conn_cnt_) {
			thread_notify_.wait();
		} else {
			Mysql_Conn* mysql_conn = new Mysql_Conn(this);
			int ret = mysql_conn->init();
			if (ret) {
				delete mysql_conn;
				thread_notify_.unlock();
				return NULL;
			} else {
				free_list_.push_back(mysql_conn);
				cur_conn_cnt_++;
				LIB_LOG_INFO("new db connection: %s, conn_cnt: %d", pool_name_.c_str(), cur_conn_cnt_);
			}
		}
	}

	Mysql_Conn* mysql_conn = free_list_.front();
	free_list_.pop_front();
	thread_notify_.unlock();

	return mysql_conn;
}

void Mysql_Pool::rel_mysql_conn(Mysql_Conn* conn) {
	thread_notify_.lock();

	std::list<Mysql_Conn*>::iterator iter = free_list_.begin();
	for (; iter != free_list_.end(); iter++) {
		if (*iter == conn) {
			break;
		}
	}

	if (iter == free_list_.end()) {
		free_list_.push_back(conn);
	}

	thread_notify_.signal();
	thread_notify_.unlock();
}

/////////////////
Mysql_Manager::Mysql_Manager() {}

Mysql_Manager::~Mysql_Manager() {
	for(boost::unordered_map<std::string, Mysql_Pool*>::iterator iter = mysql_pool_map_.begin();
			iter != mysql_pool_map_.end(); ++iter) {
		delete iter->second;
	}
	mysql_pool_map_.clear();
}

Mysql_Manager* Mysql_Manager::instance() {
	if (!instance_) {
		instance_ = new Mysql_Manager();
	}

	return instance_;
}

int Mysql_Manager::init(std::string& server_ip, int server_port, std::string& username, std::string& password, std::string& db_name, std::string& pool_name, int max_conn_cnt) {
		Mysql_Pool* mysql_pool = new Mysql_Pool(pool_name, server_ip, server_port, username, password, db_name, max_conn_cnt);

		if (mysql_pool->init()) {
			delete mysql_pool;
			LIB_LOG_FATAL("init mysql db failed, server_ip:%s, server_port:%d, username:%s, password:%s, db_name:%s, pool_name:%s",
					server_ip.c_str(), server_port, username.c_str(), password.c_str(), db_name.c_str(), pool_name.c_str());
			return -1;
		}
		mysql_pool_map_.insert(make_pair(pool_name, mysql_pool));

	return 0;
}

Mysql_Conn* Mysql_Manager::get_mysql_conn(std::string& pool_name) {
	boost::unordered_map<std::string, Mysql_Pool*>::iterator iter = mysql_pool_map_.find(pool_name);
	if (iter == mysql_pool_map_.end()) {
		return NULL;
	} else {
		return iter->second->get_mysql_conn();
	}
}

void Mysql_Manager::rel_mysql_conn(Mysql_Conn* conn) {
	if (!conn) {
		return;
	}

	boost::unordered_map<std::string, Mysql_Pool*>::iterator iter = mysql_pool_map_.find(conn->get_pool_name());
	if (iter != mysql_pool_map_.end()) {
		iter->second->rel_mysql_conn(conn);
	}
}
