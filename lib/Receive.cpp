/*
 * Receive.cpp
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#include "Receive.h"
#include "Server.h"
#include "Connector.h"

Receive_Watcher::Receive_Watcher(Receive *r, int type, int timeout)
: Epoll_Watcher(type, timeout),
  receiver_(r)
{ }

Receive_Watcher::~Receive_Watcher(void) { }

int Receive_Watcher::inner_cmd(void) {
	receiver_->process_drop();
	return 0;
}

Receive::Receive(void):
	server_(0),
	connector_(0),
	heartbeat_timeout_(Time_Value::zero),
  reactor_(0)
{ }

Receive::~Receive(void) { }

int Receive::set(Server *server, Connector *connector, Time_Value *timeout) {
	server_ = server;
	connector_ = connector;
	if (timeout == 0)
		heartbeat_timeout_ = Time_Value::zero;
	else
		heartbeat_timeout_ = *timeout;
	return 0;
}

int Receive::init(void) {
	if (heartbeat_timeout_ == Time_Value::zero) {
		if ((reactor_ = new Receive_Watcher(this)) == 0) {
			LIB_LOG_FATAL("new Receiver_Watcher");
		}
	} else {
		if ((reactor_ = new Receive_Watcher(this, WITH_IO_HEARTBEAT, heartbeat_timeout_.sec())) == 0) {
			LIB_LOG_FATAL("Receiver new Reactor");
		}
	}
	return 0;
}

int Receive::fini(void) {
	if (reactor_) {
		delete reactor_;
		reactor_ = 0;
	}
	return 0;
}

void Receive::run_handler(void) {
	LIB_LOG_DEBUG("start receiver");
	reactor_->loop();
}

Epoll_Watcher *Receive::reactor(void) {
	return reactor_;
}

int Receive::push_drop(int cid) {
	drop_list_.push_back(cid);
	reactor_->notify();
	return 0;
}

int Receive::process_drop(void) {
	int cid = 0;
	Svc *svc = 0;

	while (! drop_list_.empty()) {
		cid = drop_list_.pop_front();
		if ((svc = find_svc(cid)) != 0) {
			svc->set_closed(true);
			if (svc->get_reg_recv()) {
				svc->unregister_recv_handler();
				svc->set_reg_recv(false);
			}
		}
		drop_handler(cid);
	}

	return 0;
}

int Receive::register_svc(Svc *svc) {
	return reactor_->add(svc, EVENT_INPUT);
}

int Receive::unregister_svc(Svc *svc) {
	return reactor_->remove(svc);
}

int Receive::drop_handler(int cid) {
	LIB_LOG_TRACE("SHOULD NOT HERE");
	return 0;
}

Svc *Receive::find_svc(int cid) {
	LIB_LOG_TRACE("SHOULD NOT HERE");
	return 0;
}
