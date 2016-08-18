/*
 * Log_Struct.h
 *
 *  Created on: 2016年7月28日
 *      Author: zhangyalei
 */

#ifndef LOG_STRUCT_H_
#define LOG_STRUCT_H_

#include "Base_Struct.h"

class Log_Struct: public Base_Struct {
public:
	Log_Struct(Xml &xml, TiXmlNode *node);
	virtual ~Log_Struct();

	virtual void save_data(Block_Buffer &buffer);

private:
	void build_sql_arg(const Field_Info &field_info, Block_Buffer &buffer, std::string &str_name, std::string &str_value);
};

#endif /* LOG_SERVER_LOG_STRUCT_H_ */
