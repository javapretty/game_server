/*
 * Connect.cpp
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Common_Func.h"
#include "Connect.h"
#include "Connector.h"

Connect::Connect(void):connector_(0) { }

Connect::~Connect(void) { }

int Connect::set(Connector *connector) {
	connector_ = connector;
	return 0;
}

int Connect::connect(const char *ip, int port) {
	int connfd = 0;
	struct sockaddr_in serveraddr;

	if ((connfd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LIB_LOG_FATAL("connect socket fail, connfd = %d", connfd);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	if (::inet_pton(AF_INET, ip, &serveraddr.sin_addr) <= 0) {
		LIB_LOG_ERROR("connect inet_pton fail, ip = %s, port = %d", ip, port);
		::close(connfd);
		return -1;
	}

	if (::connect(connfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
		LIB_LOG_ERROR("connect fail, ip = %s, port = %d", ip, port);
		::close(connfd);
		return -1;
	}

	set_nonblock(connfd);

	return connect_svc(connfd);
}

int Connect::connect_svc(int connfd) {
	return 0;
}
