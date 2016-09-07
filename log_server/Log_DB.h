/*
 * Log_DB.h
 *
 *  Created on: Mar 1, 2016
 *      Author: zhangyalei
 */

#ifndef LOG_DB_H_
#define LOG_DB_H_

#include "Mysql_Conn.h"
#include "Public_Struct.h"

class Log_DB {
public:
	static Log_DB *instance(void);
	Mysql_Conn *connection(void) { return mysql_conn_; }

	int init(void);

	inline Struct_Id_Map& db_struct_id_map() { return db_struct_id_map_; }
	inline Struct_Name_Map& db_struct_name_map() { return db_struct_name_map_; }

private:
	Log_DB(void);
	virtual ~Log_DB(void);
	Log_DB(const Log_DB &);
	const Log_DB &operator=(const Log_DB &);

private:
	static Log_DB *instance_;
	Mysql_Conn* mysql_conn_;

	Struct_Id_Map db_struct_id_map_;
	Struct_Name_Map db_struct_name_map_;
};

#define LOG_DB	Log_DB::instance()

#endif /* LOG_DB_H_ */
