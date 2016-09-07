/*
 *  Created on: Dec 21, 2015
 *      Author: zhangyalei
 */

#ifndef LOGIN_MANAGER_H_
#define LOGIN_MANAGER_H_

#include "Mysql_Conn.h"
#include "Server_Manager.h"
#include "Login_Player.h"

class Login_Manager: public Server_Manager {
	typedef Object_Pool<Login_Player, Spin_Lock> Player_Pool;
public:
	static Login_Manager *instance(void);
	int init(void);

	inline Login_Player *pop_player(void) { return player_pool_.pop(); }
	inline int push_player(Login_Player *player) { return player_pool_.push(player); }

	virtual int unbind_player(Player &player);

	//发送数据
	int send_to_client(int player_cid, Block_Buffer &buf);
	int send_to_gate(int gate_cid, Block_Buffer &buf);

	//关闭连接
	int close_client(int player_cid);

	//消息处理
	int process_list();
	void process_drop_cid(int cid);
	void push_drop_cid(int cid);
	int push_tick(int x);
	int push_login_client_data(Block_Buffer *buf);
	int push_login_gate_data(Block_Buffer *buf);

	virtual int close_list_tick(Time_Value &now);
	virtual void get_server_info(void);
	virtual void free_cache(void);
	virtual void print_object_pool(void);

	int connect_mysql_db();
	int client_login(std::string& account, std::string& password);
	void get_gate_ip(std::string &account, std::string &ip, int &port);

private:
	Login_Manager(void);
	virtual ~Login_Manager(void);
	Login_Manager(const Login_Manager &);
	const Login_Manager &operator=(const Login_Manager &);

private:
	static Login_Manager *instance_;
	Player_Pool player_pool_;

	Int_List drop_cid_list_;							//掉线玩家列表
	Int_List tick_list_;										//定时器列表
	Data_List login_client_data_list_;		//client-->login
	Data_List login_gate_data_list_;			//login-->connector
	Close_List close_list_; 							// 其中的连接cid在n秒后投递到通信层关闭

	Server_Info login_client_server_info_;
	Server_Info login_gate_server_info_;

	Mysql_Conn* mysql_conn_;
};

#define LOGIN_MANAGER Login_Manager::instance()

////////////////////////////////////////////////////////////////////////////////
inline void Login_Manager::push_drop_cid(int cid) {
	drop_cid_list_.push_back(cid);
}

inline int Login_Manager::push_tick(int x) {
	tick_list_.push_back(x);
	return 0;
}

inline int Login_Manager::push_login_client_data(Block_Buffer *buf) {
	login_client_data_list_.push_back(buf);
	return 0;
}

inline int Login_Manager::push_login_gate_data(Block_Buffer *buf) {
	login_gate_data_list_.push_back(buf);
	return 0;
}

#endif /* LOGIN_MANAGER_H_ */
