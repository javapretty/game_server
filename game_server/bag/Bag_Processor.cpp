/*
 * Bag_Processor.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "Game_Client_Messager.h"
#include "Game_Player.h"

int Game_Client_Messager::process_bag_block(int msg_id, Block_Buffer &buf, Game_Player *player) {
	int ret = 0;
	switch (msg_id) {
	case REQ_FETCH_BAG_INFO: {
		MSG_120100 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_fetch_bag_info(msg);
		break;
	}
	case REQ_ADD_CAPACITY: {
		MSG_120101 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_add_capacity(msg);
		break;
	}
	case REQ_DISCARD_ITEM: {
		MSG_120102 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_discard_item(msg);
		break;
	}
	case REQ_MOVE_ITEM: {
		MSG_120103 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_move_item(msg);
		break;
	}
	case REQ_SPLIT_ITEM: {
		MSG_120104 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_split_item(msg);
		break;
	}
	case REQ_SORT_ITEM: {
		MSG_120105 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_sort_item(msg);
		break;
	}
	case REQ_MERGE_ITEM: {
		MSG_120108 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_merge_item(msg);
		break;
	}
	case REQ_SELL_ITEM: {
		MSG_120109 msg;
		if ((ret = msg.deserialize(buf)) == 0)
			player->bag().pack_sell_item(msg);
		break;
	}
	default: {
		break;
	}
	}
	return ret;
}

