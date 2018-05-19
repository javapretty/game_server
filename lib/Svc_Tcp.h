/*
 * Svc_Tcp.h
 *
 *  Created on: Apr 19,2016
 *      Author: zhangyalei
 */

#ifndef SVC_TCP_H_
#define SVC_TCP_H_

#include "Svc.h"

class Svc_Tcp : public Svc_Handler {
public:
	Svc_Tcp(void);
	virtual ~Svc_Tcp(void);

	virtual int handle_send(void);
	virtual int handle_pack(Block_Vector &block_vec);
};

#endif
