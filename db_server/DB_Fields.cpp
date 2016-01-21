/*
 * DB_Field.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: zhangyalei
 */

#include "DB_Fields.h"

const std::string Global_Fields::COLLECTION = "mmo.global";
const std::string Global_Fields::ID = "id";
const std::string Global_Fields::KEY = "key";
const std::string Global_Fields::ROLE = "role";
const std::string Global_Fields::TIME = "time";

const std::string Role_Fields::COLLECTION = "mmo.role";
const std::string Role_Fields::AGENT_NUM = "agent_num";
const std::string Role_Fields::SERVER_NUM = "server_num";
const std::string Role_Fields::ACCOUNT = "account";
const std::string Role_Fields::ROLE_ID = "role_id";
const std::string Role_Fields::ROLE_NAME = "role_name";
const std::string Role_Fields::ROLE_GENDER = "role_gender";
const std::string Role_Fields::CAREER = "career";
const std::string Role_Fields::CREATE_TIME = "create_time";
const std::string Role_Fields::LEVEL = "level";
const std::string Role_Fields::LAST_SIGN_IN_TIME = "last_sign_in_time";
const std::string Role_Fields::LAST_SIGN_OUT_TIME = "last_sign_out_time";
const std::string Role_Fields::IP = "ip";

const std::string Mail_Fields::COLLECTION = "mmo.mail";
const std::string Mail_Fields::ROLE_ID = "role_id";
const std::string Mail_Fields::TOTAL_COUNT = "total_count";
const std::string Mail_Fields::MAIL_INFO = "mail_info";
const std::string Mail_Fields::Mail_Detail::MAIL_ID = "mail_id";
const std::string Mail_Fields::Mail_Detail::PICKUP = "pickup";
const std::string Mail_Fields::Mail_Detail::SEND_TIME = "send_time";
const std::string Mail_Fields::Mail_Detail::SENDER_TYPE = "sender_type";
const std::string Mail_Fields::Mail_Detail::SENDER_ID = "sender_id";
const std::string Mail_Fields::Mail_Detail::SENDER_NAME = "sender_name";
const std::string Mail_Fields::Mail_Detail::MAIL_TITLE = "mail_title";
const std::string Mail_Fields::Mail_Detail::MAIL_CONTENT = "mail_content";
const std::string Mail_Fields::Mail_Detail::ITEM = "item";
const std::string Mail_Fields::Mail_Detail::COPPER = "copper";
const std::string Mail_Fields::Mail_Detail::BIND_COPPER = "bind_copper";
const std::string Mail_Fields::Mail_Detail::GOLD = "gold";
const std::string Mail_Fields::Mail_Detail::COUPON = "coupon";
