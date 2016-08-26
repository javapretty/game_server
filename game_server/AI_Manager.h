/*
 *  Created on: Aug 19, 2016
 *      Author: lijunliang
 */

#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include "Public_Struct.h"
#include "Behavior_Tree.h"
#include "include/v8.h"

using namespace v8;

typedef boost::unordered_map<int, Behavior_Tree *> BEHAVIOR_TREE_MAP;

class AI_Manager {
public:
	static AI_Manager *instance();
	void init();
	int thinking(Local<Object> &npc_obj, Isolate *isolate);

private:
	AI_Manager();
	~AI_Manager();
private:
	static AI_Manager *instance_;
	BEHAVIOR_TREE_MAP behavior_tree_map_;
};

#define AI_MANAGER AI_Manager::instance()

#endif
