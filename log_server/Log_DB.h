/*
 * Log_DB.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef LOG_DB_H_
#define LOG_DB_H_

#include "Mysql_Conn.h"
#include "Block_Buffer.h"
#include "DB_Struct.h"

class Log_DB {
public:
	static Log_DB *instance(void);
	Mysql_Conn *connection(void) { return mysql_conn_; }

	int init(void);

	inline DB_Struct_Id_Map& db_struct_id_map() { return db_struct_id_map_; }
	inline DB_Struct_Name_Map& db_struct_name_map() { return db_struct_name_map_; }

private:
	Log_DB(void);
	virtual ~Log_DB(void);
	Log_DB(const Log_DB &);
	const Log_DB &operator=(const Log_DB &);

private:
	static Log_DB *instance_;
	Mysql_Conn* mysql_conn_;

	DB_Struct_Id_Map db_struct_id_map_;
	DB_Struct_Name_Map db_struct_name_map_;
};

#define LOG_DB	Log_DB::instance()
#define LOG_DB_CONNECTION	LOG_DB->connection()

#endif /* LOG_DB_H_ */
