/*
 * Connect.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

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

#endif /* CONNECTOR_H_ */
