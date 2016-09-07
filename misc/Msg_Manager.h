/*
 * Msg_Manager.h
 *
 *  Created on: Aug 4, 2016
 *      Author: zhangyalei
 */

#ifndef MSG_MANAGER_H_
#define MSG_MANAGER_H_

#include "Msg_Struct.h"
#include "Public_Struct.h"

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

#endif /* MSG_MANAGER_H_ */
