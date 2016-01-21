/*
 * aa.cpp
 *
 *  Created on: Jun 7, 2012
 *      Author: ChenLong
 */

#ifndef MAILER_H_
#define MAILER_H_

#include "Public_Struct.h"
#include "Msg_Define.h"

class Game_Player;
class Mail {
public:
	Mail(void);
	virtual ~Mail(void);

	int init(Game_Player *player);
	int load_detail(Player_Data &data);
	int save_detail(Player_Data &data);
	void reset(void);

	const Mail_Info &mail_info(void) { return mail_info_;}
	void set_mail_info(const Mail_Info &mail_info) { mail_info_ = mail_info; }

  int fetch_mail_info(void);
  int pickup_mail(MSG_120201 &msg);
  int delete_mail(MSG_120202 &msg);
  int send_mail(MSG_120203 &msg);

private:
    //拾取一封邮件内的所有附件和钱
  int pickup_mail(Mail_Detail &mail_detail);
  //发送邮件
  int send_mail(role_id_t receiver_id, Mail_Detail &mail_detail);

private:
  Game_Player* player_;
	Mail_Info mail_info_;
};

#endif /* MAILER_H_ */
