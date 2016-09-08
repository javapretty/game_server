/*
 *  Created on: Aug 19, 2016
 *      Author: lijunliang
 */

#include "Xml.h"
#include "AI_Manager.h"
#include "Scene_Entity.h"
#include "Server_Config.h"

AI_Manager* AI_Manager::instance_ = NULL;

AI_Manager *AI_Manager::instance() {
	if(instance_ == NULL)
		instance_ = new AI_Manager;
	return instance_;
}

AI_Manager::AI_Manager()
{

}

AI_Manager::~AI_Manager() {

}

void AI_Manager::init() {
	Xml xml;
	const Json::Value &server_misc = SERVER_CONFIG->server_misc();
	xml.load_xml(server_misc["ai_behavior_path"].asString().c_str());
	TiXmlNode *node = xml.get_root_node();
	XML_LOOP_BEGIN(node)
		std::string label = xml.get_key(node);
		int monster_id = xml.get_attr_int(node, "id");
		Behavior_Tree *tree = new Behavior_Tree(monster_id);
		tree->init(xml, xml.enter_node(node, label.c_str()));
		behavior_tree_map_[tree->monster_id()] = tree;
	XML_LOOP_END(node)
}

int AI_Manager::thinking(Local<Object> &npc_obj, Isolate *isolate) {
	Local<String> id_name = String::NewFromUtf8(isolate, "type", NewStringType::kNormal).ToLocalChecked();
	int ai_id = npc_obj->Get(id_name)->Int32Value(isolate->GetCurrentContext()).FromMaybe(0);
	BEHAVIOR_TREE_MAP::iterator iter = behavior_tree_map_.find(ai_id);
	if(iter == behavior_tree_map_.end()){
		return -1;
	}
	Behavior_Tree *tree = iter->second;
	tree->process(npc_obj, isolate);
	return 0;
}
