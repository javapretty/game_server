#include "xml.h"

Xml::Xml():
	rootElement_(),
	doc_(new TiXmlDocument)
{
}

Xml::Xml(const char *path):
	rootElement_(),
	doc_(new TiXmlDocument)
{
	load_xml(path);
}

Xml::~Xml(){
	if(doc_){
		delete doc_;
		doc_ = NULL;
		rootElement_ = NULL;
	}
}

void Xml::load_xml(const char *path){
	doc_->LoadFile(path);
	rootElement_ = doc_->RootElement();
}

TiXmlNode *Xml::get_root_node(const char *key){
	if(rootElement_ == NULL)
		return NULL;
	if(strlen(key) > 0){
		TiXmlNode *node = rootElement_->FirstChild(key);
		if(node == NULL)
			return NULL;
		return node->FirstChild();
	}
	return rootElement_->FirstChild();
}

TiXmlNode *Xml::enter_node(TiXmlNode *node, const char *key){
	do {
		if(node->Type() != TiXmlNode::TINYXML_ELEMENT)
			continue;
		if(get_key(node) == key){
			TiXmlNode* childNode = node->FirstChild();
			do{
				if (!childNode || childNode->Type() != TiXmlNode::TINYXML_COMMENT)
					break;
			}while ((childNode = childNode->NextSibling()));
			return childNode;
		}
	} while((node = node->NextSibling()));
	return NULL;
}

std::string Xml::get_key(TiXmlNode *node){
	std::string str(node->Value());
	return str;
}

std::string Xml::get_val_str(TiXmlNode *node){
	if(node == NULL)
		return "";
	TiXmlText *ptext = node->ToText();
	std::string str(ptext->Value());
	return str;
}

int Xml::get_val_int(TiXmlNode *node){
	if(node == NULL)
		return 0;
	TiXmlText *ptext = node->ToText();
	return atoi(ptext->Value());
}

float Xml::get_val_float(TiXmlNode *node){
	if(node == NULL)
		return 0.0f;
	TiXmlText *ptext = node->ToText();
	return atof(ptext->Value());
}

std::string Xml::get_attr_str(TiXmlNode* node, const char *key){
	if(node == NULL)
		return "";
	TiXmlElement *element = node->ToElement();
	std::string value = element->Attribute(key);
	return value;
}

int Xml::get_attr_int(TiXmlNode* node, const char *key){
	if(node == NULL)
		return 0;
	TiXmlElement *element = node->ToElement();
	int value;
	element->Attribute(key, &value);
	return value;
}

float Xml::get_attr_float(TiXmlNode* node, const char *key){
	if(node == NULL)
		return 0.0f;
	TiXmlElement *element = node->ToElement();
	double value;
	element->Attribute(key, &value);
	return value;
}

