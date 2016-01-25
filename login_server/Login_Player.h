/*
 * Login_Player.h
 *
 *  Created on: 2016年1月21日
 *      Author: root
 */

#ifndef LOGIN_SERVER_LOGIN_PLAYER_H_
#define LOGIN_SERVER_LOGIN_PLAYER_H_

#include "Public_Struct.h"

class Login_Player {
public:
	Login_Player(void);
	virtual ~Login_Player(void);

	void set_cid(int cid);
	int get_cid(void);
	void set_send_client(bool send_client);


	void set_session(std::string& session);
	std::string& get_session(void);
	void set_password(std::string& password);
	std::string& get_password(void);
	void set_account(std::string& account);
	std::string& get_account(void);
	void set_gateIp(std::string& ip);
	std::string& get_gateIp(void);

	int respond_result(int msg_id, int result, Block_Buffer *buf = 0) { return respond_error_result(msg_id, result, buf); };
	int respond_finer_result(int msg_id, Block_Buffer *buf = 0);
	int respond_error_result(int msg_id, int err, Block_Buffer *buf = 0);
	/// 注: 	该接口中的buf包含头[长度、消息号、状态码], 需使用Block_Buffer::make_message/finish_message接口创建
	int send_to_client(Block_Buffer &buf);

	int sign_in(int cid, std::string account);
	int sign_out(void);
	void reset(void);

	int tick(Time_Value &now);
	void set_recycle(void);
	int recycle_status(void);
	int recycle_tick(const Time_Value &now);

	int register_timer(void);
	int unregister_timer(void);

	int link_close(void);

private:
	int cid_;
	bool is_register_timer_;
	Recycle_Tick recycle_tick_;
	std::string account_;
	std::string password_;
	std::string gete_ip_;
	std::string session_;
	bool send_client_;
};

////////////////////////////////////////////////////////////////////////////////
inline void Login_Player::set_cid(int cid) {
	cid_ = cid;
}

inline int Login_Player::get_cid(void) {
	return cid_;
}

inline void Login_Player::set_session(std::string& session) {
	session_ = session;
}

inline std::string&  Login_Player::get_session(void) {
	return session_;
}

inline int Login_Player::link_close() {
	if (recycle_tick_.status_ == Recycle_Tick::RECYCLE) return 0;

	this->set_recycle();
	return 0;
}

inline void Login_Player::set_recycle(void) {
	recycle_tick_.set(Recycle_Tick::RECYCLE);
}

inline int Login_Player::recycle_status(void) {
	return recycle_tick_.status_;
}

inline void Login_Player::set_account(std::string& account){
	account_ = account;
}

inline std::string& Login_Player::get_account(void){
	return account_;
}

inline void Login_Player::set_password(std::string& password){
	password_ = password;
}

inline std::string& Login_Player::get_password(void){
	return password_;
}

inline void Login_Player::set_gateIp(std::string& ip){
	gete_ip_= ip;
}

inline std::string& Login_Player::get_gateIp(void){
	return gete_ip_;
}

#endif /* LOGIN_SERVER_LOGIN_PLAYER_H_ */
