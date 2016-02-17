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

struct Bag_Fields {
	static const std::string COLLECTION;
	static const std::string ROLE_ID;
	static const std::string BAG_CAPACITY;
	static const std::string STORAGE_CAPACITY;

	static const std::string MONEY;
	struct Money {
		static const std::string BIND_COPPER;
		static const std::string COPPER;
		static const std::string BIND_GOLD;
		static const std::string GOLD;
	};

	static const std::string ITEM;
	struct Item {
		// 共有属性
		static const std::string INDEX;
		static const std::string ID;
		static const std::string AMOUNT;
		static const std::string BIND;
	};
};

struct Mail_Fields {
    static const std::string COLLECTION;
    static const std::string ROLE_ID;
    static const std::string TOTAL_COUNT;

    static const std::string MAIL_DETAIL;
		struct Mail_Detail {
    	static const std::string MAIL_ID;
    	static const std::string PICKUP;
    	static const std::string SEND_TIME;
		static const std::string SENDER_TYPE;
		static const std::string SENDER_ID;
		static const std::string SENDER_NAME;
		static const std::string MAIL_TITLE;
		static const std::string MAIL_CONTENT;
		static const std::string COPPER;
		static const std::string BIND_COPPER;
		static const std::string GOLD;
		static const std::string BIND_GOLD;
	};
};

#endif
