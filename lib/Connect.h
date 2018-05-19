/*
 * Connect.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef CONNECT_H_
#define CONNECT_H_

class Connector;
class Connect {
public:
	Connect(void);
	virtual ~Connect(void);

	int set(Connector *connector);
	int connect(const char *ip, int port);
	virtual int connect_svc(int connfd);

protected:
	Connector *connector_;
};

#endif /* CONNECT_H_ */
