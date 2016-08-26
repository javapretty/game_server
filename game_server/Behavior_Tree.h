/*
 *  Created on: Aug 20, 2016
 *      Author: lijunliang
 */

#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include "Public_Struct.h"
#include "Scene_Entity.h"
#include "include/v8.h"
using namespace v8;

class AI_Action {
public:
	AI_Action(std::string action);
	~AI_Action();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
	std::string action_name_;
};

class AI_Node {
public:
	typedef std::vector<AI_Node *> AI_NODE_VEC;
	AI_Node(){}
	virtual ~AI_Node(){};
	void init(Xml &xml, TiXmlNode *node);
	virtual bool process(Local<Object> &npc_obj, Isolate *isolate) = 0;
protected:
	AI_NODE_VEC action_vec_;
};

class Parallel_Node : public AI_Node {
public:
	Parallel_Node();
	~Parallel_Node();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
};

class Select_Node : public AI_Node {
public:
	Select_Node();
	~Select_Node();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
};

class Sequence_Node : public AI_Node {
public:
	Sequence_Node();
	~Sequence_Node();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
};

class Condition_Node : public AI_Node {
public:
	Condition_Node(std::string action);
	~Condition_Node();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
	AI_Action *action_;
};

class Action_Node : public AI_Node {
public:
	Action_Node(std::string action);
	~Action_Node();
	bool process(Local<Object> &npc_obj, Isolate *isolate);
private:
	AI_Action *action_;
};

class Behavior_Tree : public AI_Node {
public:
	Behavior_Tree(int monster_id);
	~Behavior_Tree();
	bool process(Local<Object> &npc_obj, Isolate *isolate);

	inline int monster_id(){return monster_id_;}
private:
	int monster_id_;
};

#endif
