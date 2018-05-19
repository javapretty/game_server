/*
 * Svc_Websocket.h
 *
 *  Created on: Apr 19,2016
 *      Author: zhangyalei
 */

#ifndef SVC_WEBSOCKET_H_
#define SVC_WEBSOCKET_H_

#include "Svc.h"

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

class Svc_Websocket : public Svc_Handler {
public:
	Svc_Websocket(void);
	virtual ~Svc_Websocket(void);

	void reset(void);
	virtual int handle_send(void);
	virtual int handle_pack(Block_Vector &block_vec);

private:
	//websocket第一步握手
	int handshake(Block_Buffer *buffer);
	//解析websocket帧头，获取buffer内容
	Block_Buffer *get_frame_buffer(int16_t payload_length, uint8_t *masking_key, Block_Buffer *buffer);
	//包装websocket帧头
	Block_Buffer *make_frame_buffer(Block_Buffer *buffer, uint8_t *op = NULL);

private:
	bool connected_;
};

#endif
