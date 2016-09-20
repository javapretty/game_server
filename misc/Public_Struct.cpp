/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#include "Public_Struct.h"
#include "Server_Config.h"
#include "Log.h"
#include "Mongo_Struct.h"
#include "Mysql_Struct.h"
#include "Log_Struct.h"
#include "Msg_Struct.h"

int load_struct(const char *path, Struct_Type struct_type, Struct_Id_Map &struct_id_map, Struct_Name_Map &struct_name_map){
	Xml xml;
	xml.load_xml(path);
	TiXmlNode *node = xml.get_root_node();
	XML_LOOP_BEGIN(node)
		Base_Struct *base_struct = nullptr;
		if (struct_type == MONGO_STRUCT)	{
			base_struct = new Mongo_Struct(xml, node);
		} else if (struct_type == MYSQL_STRUCT) {
			base_struct = new Mysql_Struct(xml, node);
		} else if (struct_type == LOG_STRUCT) {
			base_struct = new Log_Struct(xml, node);
		} else if (struct_type == MSG_STRUCT) {
			base_struct = new Msg_Struct(xml, node);
		} else {
			LOG_FATAL("load_struct struct_type = %d error abort", struct_type);
		}

		if(base_struct->msg_id() > 0) {
			struct_id_map.insert(std::pair<int32_t, Base_Struct*>(base_struct->msg_id(), base_struct));
		}
		struct_name_map.insert(std::pair<std::string, Base_Struct*>(base_struct->struct_name(), base_struct));
	XML_LOOP_END(node)
	return 0;
}

void Server_Conf::init_server_conf(void) {
	const Json::Value &server_conf = SERVER_CONFIG->server_conf();

	server_sleep_time = Time_Value(1, 0);					//1s
	receive_timeout = Time_Value(server_conf["receive_timeout"].asInt(), 0);	//900s
	server_send_interval = Time_Value(0, server_conf["server_send_interval"].asInt());				//100us
	connector_send_interval = Time_Value(0, server_conf["connector_send_interval"].asInt());	//100us

	//log_server
	log_server.server_id = server_conf["log_server"]["server_id"].asInt();
	log_server.server_ip = server_conf["log_server"]["server_ip"].asString();
	log_server.server_port = server_conf["log_server"]["server_port"].asInt();
	log_server.network_protocol = server_conf["log_server"]["network_protocol"].asInt();

	//db_server
	db_server.server_id = server_conf["db_server"]["server_id"].asInt();
	db_server.server_ip = server_conf["db_server"]["server_ip"].asString();
	db_server.server_port = server_conf["db_server"]["server_port"].asInt();
	db_server.network_protocol = server_conf["db_server"]["network_protocol"].asInt();

	//login_server
	login_client_server.server_id = server_conf["login_client_server"]["server_id"].asInt();
	login_client_server.server_ip = server_conf["login_client_server"]["server_ip"].asString();
	login_client_server.server_port = server_conf["login_client_server"]["server_port"].asInt();
	login_client_server.network_protocol = server_conf["login_client_server"]["network_protocol"].asInt();

	login_gate_server.server_id = server_conf["login_gate_server"]["server_id"].asInt();
	login_gate_server.server_ip = server_conf["login_gate_server"]["server_ip"].asString();
	login_gate_server.server_port = server_conf["login_gate_server"]["server_port"].asInt();
	login_gate_server.network_protocol = server_conf["login_gate_server"]["network_protocol"].asInt();

	//master_server
	master_gate_server.server_id = server_conf["master_gate_server"]["server_id"].asInt();
	master_gate_server.server_ip = server_conf["master_gate_server"]["server_ip"].asString();
	master_gate_server.server_port = server_conf["master_gate_server"]["server_port"].asInt();
	master_gate_server.network_protocol = server_conf["master_gate_server"]["network_protocol"].asInt();

	master_game_server.server_id = server_conf["master_game_server"]["server_id"].asInt();
	master_game_server.server_ip = server_conf["master_game_server"]["server_ip"].asString();
	master_game_server.server_port = server_conf["master_game_server"]["server_port"].asInt();
	master_game_server.network_protocol = server_conf["master_game_server"]["network_protocol"].asInt();

	master_http_server.server_id = server_conf["master_http_server"]["server_id"].asInt();
	master_http_server.server_ip = server_conf["master_http_server"]["server_ip"].asString();
	master_http_server.server_port = server_conf["master_http_server"]["server_port"].asInt();
	master_http_server.network_protocol = server_conf["master_http_server"]["network_protocol"].asInt();

	//game_server
	for(uint i = 0; i < server_conf["game_server"].size(); i++){
		Server_Detail server_detail;
		server_detail.server_id = server_conf["game_server"][i]["server_id"].asInt();
		server_detail.server_ip = server_conf["game_server"][i]["server_ip"].asString();
		server_detail.server_port = server_conf["game_server"][i]["server_port"].asInt();
		server_detail.network_protocol = server_conf["game_server"][i]["network_protocol"].asInt();
		game_list.push_back(server_detail);
	}

	//gate_server
	for(uint i = 0; i < server_conf["gate_server"].size(); i++){
		Server_Detail server_detail;
		server_detail.server_id = server_conf["gate_server"][i]["server_id"].asInt();
		server_detail.server_ip = server_conf["gate_server"][i]["server_ip"].asString();
		server_detail.server_port = server_conf["gate_server"][i]["server_port"].asInt();
		server_detail.network_protocol = server_conf["gate_server"][i]["network_protocol"].asInt();
		gate_list.push_back(server_detail);
	}
}

Position::Position():
		x(0),
		y(0),
		z(0)
{

}

Position::Position(int posx, int posy, int posz):
		x(posx),
		y(posy),
		z(posz)
{
}

Position::~Position(){

}

float Position::operator-(Position &pos){
		return (float)sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2) + pow(z - pos.z, 2));
}

void Position::operator=(Position &pos){
		x = pos.x;
		y = pos.y;
		z = pos.z;
}

void Position::set_position(int posx, int posy, int posz){
		x = posx;
		y = posy;
		z = posz;
}

void Position::set_position(Position &pos){
	set_position(pos.x, pos.y, pos.z);
}

void Position::reset() {
	x = 0;y = 0;z = 0;
}
