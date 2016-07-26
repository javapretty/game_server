/*
 * Log_DB.cpp
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include "Log_DB.h"
#include "Msg_Define.h"
#include "Log.h"

Log_DB::Log_DB(void):
  mysql_conn_(NULL)
{ }


Log_DB::~Log_DB(void) {
	MYSQL_MANAGER->rel_mysql_conn(mysql_conn_);
}

int Log_DB::init(std::string &ip, int port, std::string &user, std::string &password, std::string &dbname, std::string &dbpoolname) {
	mysql_poolname_ = dbpoolname;
	MYSQL_MANAGER->init(ip, port, user, password, dbname, dbpoolname, 16);
	mysql_conn_ = MYSQL_MANAGER->get_mysql_conn(mysql_poolname_);

	return 0;
}

int Log_DB::process_180001(int msg_id, int status, Block_Buffer &buf) {
	MSG_180001 msg;
	msg.deserialize(buf);
	char str_sql[256] = {0};
	sprintf(str_sql, "INSERT INTO logout(`role_id`,`role_name`,`account`,`level`,`client_ip`,`login_time`,`logout_time`) VALUES(%ld, \'%s\', \'%s\', %d, \'%s\', %d, %d)",
			msg.role_id, msg.role_name.c_str(), msg.account.c_str(), msg.level, msg.client_ip.c_str(), msg.login_time, msg.logout_time);
	mysql_conn_->execute(str_sql);
	return 0;
}
