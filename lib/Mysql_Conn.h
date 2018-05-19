/*
 * Mysql_Conn.h
 *
 *  Created on: Jan 4,2016
 *      Author: zhangyalei
 */

#ifndef LIB_MYSQL_CONN_H_
#define LIB_MYSQL_CONN_H_

#include "boost/unordered_map.hpp"
#include "Thread_Mutex.h"
#include <string>
#include <list>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DataBuf : public std::streambuf
{
public:
	DataBuf(char* buf, size_t len) {
		setg(buf, buf, buf+len);
    }
};

class Mysql_Pool;
class Mysql_Conn {
public:
	Mysql_Conn(Mysql_Pool* mysql_pool);
	virtual ~Mysql_Conn();

	int init(void);
	sql::PreparedStatement* create_pstmt(const char* str_sql);
	std::string& get_pool_name();

	sql::ResultSet* execute_query(const char* str_sql);
	int execute_update(const char* str_sql);
	int execute(const char* str_sql);

private:
	Mysql_Pool* mysql_pool_;
	sql::Connection*  conn_;
	sql::Statement* stmt_ ;
};

class Mysql_Pool {
public:
	Mysql_Pool(std::string& pool_name,  std::string& server_ip, uint32_t server_port,
			std::string& username,  std::string& password,  std::string& db_name, int32_t max_conn_cnt);
	virtual ~Mysql_Pool();

	int init(void);
	Mysql_Conn* get_mysql_conn();
	void rel_mysql_conn(Mysql_Conn* conn);

	inline std::string& get_pool_name() { return pool_name_; }
	inline std::string& get_server_ip() { return server_ip_; }
	inline int32_t get_server_port() { return server_port_; }
	inline std::string& get_username() { return username_; }
	inline std::string& get_passwrod() { return password_; }
	inline std::string& get_db_name() { return db_name_; }
	inline sql::Driver* get_driver(){return driver_;}

private:
	std::string 		pool_name_;
	std::string 		server_ip_;
	int32_t				server_port_;
	std::string 		username_;
	std::string 		password_;
	std::string 		db_name_;
	int32_t				cur_conn_cnt_;
	int32_t 				max_conn_cnt_;

	sql::Driver*  driver_;
	std::list<Mysql_Conn*>	free_list_;
	Thread_Notify	 thread_notify_;
};

class Mysql_Manager {
public:
	static Mysql_Manager* instance();
	int init(std::string& server_ip, int server_port, std::string& username, std::string& password, std::string& db_name, std::string& pool_name, int max_conn_cnt);

	Mysql_Conn* get_mysql_conn(std::string& pool_name);
	void rel_mysql_conn(Mysql_Conn* conn);

private:
	Mysql_Manager();
	virtual ~Mysql_Manager();

private:
	static Mysql_Manager*	instance_;
	boost::unordered_map<std::string, Mysql_Pool*>	 mysql_pool_map_;
};

#define	MYSQL_MANAGER Mysql_Manager::instance()

#endif /* LIB_MYSQL_CONN_H_ */
