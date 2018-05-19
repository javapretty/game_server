/*
 * Svc_Http.h
 *
 *  Created on: Aug 16,2016
 *      Author: zhangyalei
 */

#ifndef SVC_HTTP_H_
#define SVC_HTTP_H_

#include "Svc.h"

#define HTTP_RESPONSE_HTML          "HTTP/1.1 200 OK\r\n"\
                                    "Connection:close\r\n"\
                                    "Content-Length:%d\r\n"\
                                    "Content-Type:text/html;charset=utf-8\r\n\r\n%s"

class Svc_Http : public Svc_Handler {
public:
	Svc_Http(void);
	virtual ~Svc_Http(void);

	virtual int handle_send(void);
	virtual int handle_pack(Block_Vector &block_vec);

private:
	void make_http_head(Block_Buffer *buf);
};

#endif /* SVC_HTTP_H_ */
