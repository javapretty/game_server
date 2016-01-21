/*
 * aa.cpp
 *
 *  Created on: Jun 7, 2012
 *      Author: ChenLong
 */

#include "Game_Client_Messager.h"
#include "Game_Player.h"

int Game_Client_Messager::process_mail_block(int msg_id, Block_Buffer &buf, Game_Player *player) {
	int ret = 0;
	switch (msg_id) {
	case REQ_FETCH_MAIL_INFO: {
		player->mail().fetch_mail_info();
		break;
	}
	case REQ_PICKUP_MAIL: {
		MSG_120201 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->mail().pickup_mail(msg);
		break;
	}
	case REQ_DEL_MAIL: {
		MSG_120202 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->mail().delete_mail(msg);
		break;
	}
	case REQ_SEND_MAIL: {
		MSG_120203 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->mail().send_mail(msg);
		break;
	}
	default: {
		break;
	}
	}
	return ret;
}


