// -*- C++ -*-
/*
 * My_DB_Fields.h
 *
 *  Created on: Nov 19, 2012
 *      Author: zhangyalei
 */

#ifndef MY_DB_FIELDS_H_
#define MY_DB_FIELDS_H_

#include "Time_Value.h"
#include <stdint.h>
#include <string>
#include <string.h>
#include <sstream>

struct Data_Collector {
	std::string insert_head_; 		/// SQL INSERT语句头, 包含INSERT INTO Table VALUES, 不包含其后的()中的数据
	std::string sql_str_; 			/// 拼接出来的SQL语句
	size_t num_, max_num_; 			/// 当前收集器中的数据条数和最大条数
	Time_Value first_data_timestamp_, timeout_tv_;

	Data_Collector(void) {
		reset();
	}

	inline void set(std::string &insert_head, size_t max_num, const Time_Value &timeout_tv) {
		insert_head_ = insert_head;
		sql_str_ = insert_head_;
		max_num_ = max_num;
		timeout_tv_ = timeout_tv;
	}

	inline void reset_used(void) {
		sql_str_ = insert_head_;
		num_ = 0;
		first_data_timestamp_ = Time_Value::zero;
	}

	inline void reset(void) {
		insert_head_.clear();
		sql_str_.clear();
		num_ = max_num_ = 0;
		first_data_timestamp_ = timeout_tv_ = Time_Value::zero;
	}

	bool is_timeout(Time_Value &now) {
		return (num_ != 0 && (now - first_data_timestamp_ > timeout_tv_));
	}
};

template <typename FT>
struct My_Fields {
	FT data_;
	bool is_null_;

	My_Fields(void) : is_null_(false) { }

	void reset(void) {
		is_null_ = false;
	}
};

struct Table {
	Table(void) { }
	virtual ~Table(void) { }

	void append_insert_content(Data_Collector &collector) {
		if (collector.num_ == 0) {
			collector.first_data_timestamp_ = Time_Value::gettimeofday();
		} else {
			collector.sql_str_ += ",";
		}
		++collector.num_;

		append_insert_handler(collector);
	}

	virtual void append_insert_handler(Data_Collector &collector) = 0;
};

struct Test_Table: public Table {
	My_Fields<int> role_id_;
	My_Fields<std::string> role_name_;

	virtual void append_insert_handler(Data_Collector &collector);
};

struct Loginout_Stream: public Table {
	My_Fields<int64_t> role_id_;
	My_Fields<std::string> role_name_;
	My_Fields<std::string> account_;
	My_Fields<uint16_t> level_;
	My_Fields<std::string> client_ip_;
	My_Fields<uint32_t> login_time_;
	My_Fields<uint32_t> logout_time_;
	My_Fields<uint32_t> online_time_;

	virtual void append_insert_handler(Data_Collector &collector);
};

#endif /* MY_DB_FIELDS_H_ */
