/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */

#ifndef GATE_MANAGER_H_
#define GATE_MANAGER_H_

#include "Server_Manager.h"
#include "Gate_Player.h"

class Gate_Game_Connector;
struct Gate_Game_Connector_Info {
	Gate_Game_Connector *connector;
	int game_cid;
	int game_server_id;
	int game_player_num;
};

class Gate_Manager: public Server_Manager {
	typedef Object_Pool<Gate_Player, Spin_Lock> Player_Pool;
	typedef boost::unordered_map<int, Gate_Game_Connector_Info> Game_Connector_Map;
public:
	static Gate_Manager *instance(void);
	int init(int server_id);

	inline bool verify_pack(void) { return verify_pack_onoff_; }
	Gate_Player *pop_player(void) { return player_pool_.pop(); }
	int push_player(Gate_Player *player) { return player_pool_.push(player); }

	virtual int unbind_player(Player &player);

	//发送数据
	int send_to_client(int player_cid, Block_Buffer &buf);
	int send_to_game(int cid, Block_Buffer &buf);
	int send_to_login(Block_Buffer &buf);
	int send_to_master(Block_Buffer &buf);

	//关闭客户端连接
	int close_client(int player_cid);

	//消息处理
	int process_list();
	void process_drop_cid(int cid);
	void push_drop_cid(int cid);
	int push_tick(int x);
	int push_gate_client_data(Block_Buffer *buf);
	int push_gate_login_data(Block_Buffer *buf);
	int push_gate_game_data(Block_Buffer *buf);
	int push_gate_master_data(Block_Buffer *buf);

	virtual int close_list_tick(Time_Value &now);
	virtual void get_server_info(void);
	virtual void free_cache(void);
	virtual void print_object_pool(void);

	void get_server_ip_port(int player_cid, std::string &ip, int &port);
	void new_game_connector(int server_id, std::string server_ip, int server_port, Time_Value &send_interval);
	int get_game_cid(int server_id);
	int get_lowest_player_game_cid(void);

private:
	Gate_Manager(void);
	virtual ~Gate_Manager(void);
	Gate_Manager(const Gate_Manager &);
	const Gate_Manager &operator=(const Gate_Manager &);

private:
	static Gate_Manager *instance_;

	int server_id_;
	bool verify_pack_onoff_;
	Player_Pool player_pool_;

	Int_List drop_cid_list_;							//掉线玩家列表
	Int_List tick_list_;									//定时器列表
	Data_List gate_client_data_list_;			//client-->gate
	Data_List gate_login_data_list_;				//login-->gate
	Data_List gate_game_data_list_;				//game-->gate
	Data_List gate_master_data_list_;			//master-->gate
	Close_List close_list_; 							//其中的连接cid在n秒后投递到通信层关闭

	Server_Info gate_client_server_info_;

	Game_Connector_Map game_connector_map_;
};

#define GATE_MANAGER Gate_Manager::instance()

////////////////////////////////////////////////////////////////////////////////

inline void Gate_Manager::push_drop_cid(int cid) {
	drop_cid_list_.push_back(cid);
}

inline int Gate_Manager::push_tick(int x) {
	tick_list_.push_back(x);
	return 0;
}

inline int Gate_Manager::push_gate_client_data(Block_Buffer *buf) {
	gate_client_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_login_data(Block_Buffer *buf) {
	gate_login_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_game_data(Block_Buffer *buf) {
	gate_game_data_list_.push_back(buf);
	return 0;
}

inline int Gate_Manager::push_gate_master_data(Block_Buffer *buf) {
	gate_master_data_list_.push_back(buf);
	return 0;
}

#endif /* GATE_MANAGER_H_ */
