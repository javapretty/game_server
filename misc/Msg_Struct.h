/*
 * Msg_Struct.h
 *
 *  Created on: 2016年8月2日
 *      Author: zhangyalei
 */

#ifndef MSG_STRUCT_H_
#define MSG_STRUCT_H_

#include "Base_Struct.h"

class Msg_Struct: public Base_Struct {
public:
	Msg_Struct(Xml &xml, TiXmlNode *node) : Base_Struct(xml, node) {}
	virtual ~Msg_Struct() {}
};

class Msg_Manager {
public:
	static Msg_Manager *instance(void);

	int init(void);

	inline Struct_Id_Map& msg_struct_id_map() { return msg_struct_id_map_; }
	inline Struct_Name_Map& msg_struct_name_map() { return msg_struct_name_map_; }

private:
	Msg_Manager(void);
	virtual ~Msg_Manager(void);
	Msg_Manager(const Msg_Manager &);
	const Msg_Manager &operator=(const Msg_Manager &);

private:
	static Msg_Manager *instance_;

	Struct_Id_Map msg_struct_id_map_;
	Struct_Name_Map msg_struct_name_map_;
};

#define MSG_MANAGER	Msg_Manager::instance()

#endif /* MSG_STRUCT_H_ */
