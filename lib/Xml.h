/*
 * Xml.h
 *
 *  Created on: Jul 15,2016
 *      Author: zhangyalei
 */

#ifndef XML_H_
#define XML_H_

#include "tinyxml.h"
#include <string>

#define XML_LOOP_BEGIN(NODE)\
	do { \
		if(NODE->Type() != TiXmlNode::TINYXML_ELEMENT) \
			continue;

#define XML_LOOP_END(NODE)\
	} while ((NODE = NODE->NextSibling()));

class Xml {
public:
	Xml();
	Xml(const char *path);
	~Xml();
	void load_xml(const char *path);

	TiXmlNode *get_root_node(const char *key = "");
	TiXmlNode *enter_node(TiXmlNode *node, const char *key);

	std::string get_key(TiXmlNode *node);
	bool has_key(TiXmlNode *node, const char *key);

	std::string get_val_str(TiXmlNode *node);
	int get_val_int(TiXmlNode *node);
	float get_val_float(TiXmlNode *node);

	std::string get_attr_str(TiXmlNode* node, const char *key = NULL);
	int get_attr_int(TiXmlNode* node, const char *key = NULL);
	float get_attr_float(TiXmlNode* node, const char *key = NULL);

private:
	TiXmlElement *rootElement_;
	TiXmlDocument *doc_;
};

#endif
