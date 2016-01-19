/*
 * DB_Fields.h
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#ifndef DB_FIELDS_H_
#define DB_FIELDS_H_

#include <string>

struct Global_Fields {
    static const std::string COLLECTION;

    static const std::string ID;
    static const std::string KEY;
    static const std::string ROLE;
    static const std::string TIME;

};

struct Role_Fields {
	static const std::string COLLECTION;

	static const std::string AGENT_NUM;
	static const std::string SERVER_NUM;
	static const std::string ACCOUNT;
	static const std::string ROLE_ID;
	static const std::string ROLE_NAME;
	static const std::string ROLE_GENDER;
	static const std::string CAREER;
	static const std::string CREATE_TIME;
	static const std::string LEVEL;
	static const std::string LAST_SIGN_IN_TIME;
	static const std::string LAST_SIGN_OUT_TIME;
	static const std::string IP;
};

#endif
