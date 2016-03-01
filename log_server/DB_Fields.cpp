/*
 * My_DB_Fields.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#include <DB_Fields.h>
#include "DB_Record.h"

void Test_Table::append_insert_handler(Data_Collector &collector) {
	collector.sql_str_ += "(";

	std::stringstream stream;

	/// role_id
	if (role_id_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (role_id_.data_);
		collector.sql_str_ += stream.str();
	}

	/// role_name
	collector.sql_str_ += ",";
	if (role_name_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << "\"" << role_name_.data_ << "\"";
		collector.sql_str_ += stream.str();
	}

	collector.sql_str_ += ")";
}

void Loginout_Stream::append_insert_handler(Data_Collector &collector) {
	collector.sql_str_ += "(";

	std::stringstream stream;

	/// role_id_
	if (role_id_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (role_id_.data_);
		collector.sql_str_ += stream.str();
	}
	/// role_name_
	collector.sql_str_ += ",";
	if (role_name_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << "\"" << role_name_.data_ << "\"";
		collector.sql_str_ += stream.str();
	}
	/// account_
	collector.sql_str_ += ",";
	if (account_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << "\"" << account_.data_ << "\"";
		collector.sql_str_ += stream.str();
	}
	/// level_
	collector.sql_str_ += ",";
	if (level_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (level_.data_);
		collector.sql_str_ += stream.str();
	}
	/// client_ip_
	collector.sql_str_ += ",";
	if (client_ip_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << "\"" << client_ip_.data_ << "\"";
		collector.sql_str_ += stream.str();
	}
	/// login_time_
	collector.sql_str_ += ",";
	if (login_time_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (login_time_.data_);
		collector.sql_str_ += stream.str();
	}
	/// logout_time_
	collector.sql_str_ += ",";
	if (logout_time_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (logout_time_.data_);
		collector.sql_str_ += stream.str();
	}
	/// online_time_
	collector.sql_str_ += ",";
	if (online_time_.is_null_) {
		collector.sql_str_ += "NULL";
	} else {
		stream.str("");
		stream << (online_time_.data_);
		collector.sql_str_ += stream.str();
	}

	collector.sql_str_ += ")";
}
