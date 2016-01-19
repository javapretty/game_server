/*
 * Mysql_Conn.h
 *
 *  Created on: 2016年1月4日
 *      Author: zhangyalei
 */

#ifndef LIB_MYSQL_CONN_H_
#define LIB_MYSQL_CONN_H_

#include "Thread_Mutex.h"
#include <string>
#include <map>
#include <list>
#include <stdint.h>
#include <stdlib.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace sql {
class Driver;
class Connection;
class ResultSet;
class Statement;
}

class Mysql_DB_Pool;

class Mysql_DB_Conn {
public:
	Mysql_DB_Conn(Mysql_DB_Pool* pDBPool);
	virtual ~Mysql_DB_Conn();
	int Init();

	sql::ResultSet* ExecuteQuery(const char* sql_query);
	int ExecuteUpdate(const char* sql_query);
	bool Execute(const char* sql_query);

	std::string& GetPoolName();

private:
	Mysql_DB_Pool* 	m_pDBPool;	// to get MySQL server information
	sql::Connection*  conn;
	sql::Statement* stmt ;
};

class Mysql_DB_Pool {
public:
	Mysql_DB_Pool(std::string& pool_name,  std::string& db_server_ip, uint32_t db_server_port,
			std::string& username,  std::string& password,  std::string& db_name, int32_t max_conn_cnt);
	virtual ~Mysql_DB_Pool();

	int Init();
	Mysql_DB_Conn* GetDBConn();
	void RelDBConn(Mysql_DB_Conn* pConn);

	inline std::string& GetPoolName() { return m_pool_name; }
	inline std::string& GetDBServerIP() { return m_db_server_ip; }
	inline uint32_t GetDBServerPort() { return m_db_server_port; }
	inline std::string& GetUsername() { return m_username; }
	inline std::string& GetPasswrod() { return m_password; }
	inline std::string& GetDBName() { return m_db_name; }
	inline sql::Driver* GetDriver(){return driver;}

private:
	std::string 		m_pool_name;
	std::string 		m_db_server_ip;
	uint32_t				m_db_server_port;
	std::string 		m_username;
	std::string 		m_password;
	std::string 		m_db_name;
	int32_t				m_db_cur_conn_cnt;
	int32_t 				m_db_max_conn_cnt;

	sql::Driver*  driver;

	std::list<Mysql_DB_Conn*>	m_free_list;
	Thread_Notify	 m_thread_notify;
};

class Mysql_DB_Manager {
public:
	static Mysql_DB_Manager* instance();
	int Init(std::string& db_host, int db_port, std::string& db_username, std::string& db_password, std::string& db_name, std::string& pool_name, int db_maxconncnt);

	Mysql_DB_Conn* GetDBConn(std::string& dbpool_name);
	void RelDBConn(Mysql_DB_Conn* pConn);

private:
	Mysql_DB_Manager();
	virtual ~Mysql_DB_Manager();

private:
	static Mysql_DB_Manager*		mysql_db_manager;
	std::map<std::string, Mysql_DB_Pool*>	 m_dbpool_map;
};

#define	MYSQL_DB_MANAGER Mysql_DB_Manager::instance()

#endif /* LIB_MYSQL_CONN_H_ */
