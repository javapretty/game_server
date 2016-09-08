/*
 *  Created on: Aug 20, 2016
 *      Author: lijunliang
 */

#include "Log.h"
#include "Behavior_Tree.h"

AI_Action::AI_Action(std::string action):
	action_name_(action) {

}

AI_Action::~AI_Action() {

}

bool AI_Action::process(Local<Object> &npc_obj, Isolate *isolate) {
	Local<String> func_name = String::NewFromUtf8(isolate, (action_name_ + "_handler").c_str(), NewStringType::kNormal).ToLocalChecked();
	Local<Value> func_value = npc_obj->Get(func_name);
	if(func_value->IsUndefined()){
		LOG_ERROR("function %s_handler is undefined", action_name_.c_str());
		return false;
	}
	Local<Function> func = Local<Function>::Cast(func_value);
	Local<Value> ret = func->Call(npc_obj, 0, NULL);
	return ret->IsUndefined() ? true : ret->BooleanValue(isolate->GetCurrentContext()).FromMaybe(0);
}

void AI_Node::init(Xml &xml, TiXmlNode *node) {
	AI_Node *ai_node = NULL;
	XML_LOOP_BEGIN(node)
		std::string label = xml.get_key(node);
		if(label == "parallel"){
			ai_node = new Parallel_Node();
			ai_node->init(xml, xml.enter_node(node, "parallel"));
		}
		else if(label == "select"){
			ai_node = new Select_Node();
			ai_node->init(xml, xml.enter_node(node, "select"));
		}
		else if(label == "sequence"){
			ai_node = new Sequence_Node();
			ai_node->init(xml, xml.enter_node(node, "sequence"));
		}
		else if(label == "condition"){
			ai_node = new Condition_Node(xml.get_attr_str(node, "type"));
		}
		else if(label == "action"){
			ai_node = new Action_Node(xml.get_attr_str(node, "type"));
		}
		action_vec_.push_back(ai_node);
	XML_LOOP_END(node)
}

Parallel_Node::Parallel_Node(){}

Parallel_Node::~Parallel_Node(){}

bool Parallel_Node::process(Local<Object> &npc_obj, Isolate *isolate) {
	for(AI_NODE_VEC::iterator iter = action_vec_.begin();
			iter != action_vec_.end(); iter++){
		AI_Node *node = (*iter);
		node->process(npc_obj, isolate);
	}
	return true;
}

Select_Node::Select_Node(){}

Select_Node::~Select_Node(){}

bool Select_Node::process(Local<Object> &npc_obj, Isolate *isolate) {
	for(AI_NODE_VEC::iterator iter = action_vec_.begin();
			iter != action_vec_.end(); iter++){
		AI_Node *node = (*iter);
		if(node->process(npc_obj, isolate)){
			return true;
		}
	}
	return false;
}

Sequence_Node::Sequence_Node(){}

Sequence_Node::~Sequence_Node(){}

bool Sequence_Node::process(Local<Object> &npc_obj, Isolate *isolate) {
	for(AI_NODE_VEC::iterator iter = action_vec_.begin();
			iter != action_vec_.end(); iter++){
		AI_Node *node = (*iter);
		if(!node->process(npc_obj, isolate)) {
			return false;
		}
	}
	return true;
}

Condition_Node::Condition_Node(std::string action):
		action_(new AI_Action(action))
{

}

Condition_Node::~Condition_Node(){}

bool Condition_Node::process(Local<Object> &npc_obj, Isolate *isolate) {
	return action_->process(npc_obj, isolate);
}

Action_Node::Action_Node(std::string action):
		action_(new AI_Action(action))
{

}

Action_Node::~Action_Node(){}

bool Action_Node::process(Local<Object> &npc_obj, Isolate *isolate) {
	return action_->process(npc_obj, isolate);
}

Behavior_Tree::Behavior_Tree(int monster_id):
		monster_id_(monster_id)
{

}

Behavior_Tree::~Behavior_Tree() {

}

bool Behavior_Tree::process(Local<Object> &npc_obj, Isolate *isolate) {
	for(AI_NODE_VEC::iterator iter = action_vec_.begin();
			iter != action_vec_.end(); iter++){
		AI_Node *node = (*iter);
		node->process(npc_obj, isolate);
	}
	return true;
}
