/*
 * Connector.cpp
 *
 *  Created on: Jan 16,2016
 *      Author: zhangyalei
 */

#include "Connector.h"

int Connector_Connect::connect_svc(int connfd) {
	LIB_LOG_DEBUG("connfd=%d", connfd);

	Connector_Svc *svc = connector_->svc_pool().pop();
	if (! svc) {
		LIB_LOG_ERROR("svc == NULL");
		return -1;
	}

	int cid = connector_->svc_list().record_svc(svc);
	if (cid == -1) {
		LIB_LOG_ERROR("cid == -1");
		connector_->svc_pool().push(svc);
		return -1;
	}

	svc->reset();
	svc->create_handler(TCP);
	svc->set_cid(cid);
	svc->set_fd(connfd);
	svc->set_peer_addr();
	svc->set_connector(connector_);

	svc->register_recv_handler();
	svc->register_send_handler();

	return cid;
}

////////////////////////////////////////////////////////////////////////////////

int Connector_Receive::drop_handler(int cid) {
	return connector_->send().push_drop(cid);
}

Svc *Connector_Receive::find_svc(int cid) {
	return connector_->find_svc(cid);
}

////////////////////////////////////////////////////////////////////////////////


Block_Buffer *Connector_Send::pop_block(int cid) {
	return connector_->pop_block(cid);
}

int Connector_Send::push_block(int cid, Block_Buffer *buffer) {
	return connector_->push_block(cid, buffer);
}

int Connector_Send::drop_handler(int cid) {
	LIB_LOG_INFO("connector drop_handler, cid = %d", cid);
	return connector_->recycle_svc(cid);
}

Svc *Connector_Send::find_svc(int cid) {
	return connector_->find_svc(cid);
}

////////////////////////////////////////////////////////////////////////////////

Block_Buffer *Connector_Svc::pop_block(int cid) {
	return connector_->pop_block(cid);
}

int Connector_Svc::push_block(int cid, Block_Buffer *buffer) {
	return connector_->push_block(cid, buffer);
}

int Connector_Svc::post_block(Block_Buffer* buffer) {
	return connector_->block_list().push_back(buffer);
}

int Connector_Svc::register_recv_handler(void) {
	if (! get_reg_recv()) {
		connector_->receive().register_svc(this);
		set_reg_recv(true);
	}
	return 0;
}

int Connector_Svc::unregister_recv_handler(void) {
	if (get_reg_recv()) {
		connector_->receive().unregister_svc(this);
		set_reg_recv(false);
	}
	return 0;
}

int Connector_Svc::register_send_handler(void) {
	if (! get_reg_send()) {
		connector_->send().register_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Connector_Svc::unregister_send_handler(void) {
	if (get_reg_send()) {
		connector_->send().unregister_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Connector_Svc::close_handler(int cid) {
	connector_->receive().push_drop(cid);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

Connector::Connector(void):cid_(-1),server_port_(0) { }

Connector::~Connector(void) { }

void Connector::run_handler(void) {
	process_list();
}

void Connector::process_list(void) {
	LIB_LOG_TRACE("SHOULD NOT HERE");
}

void Connector::set(std::string &server_ip, int server_port, Time_Value &send_interval) {
	server_ip_ = server_ip;
	server_port_ = server_port;
	connect_.set(this);
	receive_.set(0, this);
	send_.set(0, this, send_interval);
}

int Connector::init(void) {
	receive_.init();
	send_.init();
	return 0;
}

int Connector::start(void) {
	receive_.thr_create();
	send_.thr_create();
	return 0;
}

int Connector::connect_server(void) {
	cid_ = connect_.connect(server_ip_.c_str(), server_port_);
	LIB_LOG_DEBUG("cid_ = %d", cid_);
	return cid_;
}

Block_Buffer *Connector::pop_block(int cid) {
	return block_pool_group_.pop_block(cid);
}

int Connector::push_block(int cid, Block_Buffer *buffer) {
	return block_pool_group_.push_block(cid, buffer);
}

int Connector::send_block(int cid, Block_Buffer &buffer) {
	return send_.push_svc_block(cid, buffer);
}

Svc *Connector::find_svc(int cid) {
	Connector_Svc *svc = 0;
	svc_static_list_.get_used_svc(cid, svc);
	return svc;
}

int Connector::recycle_svc(int cid) {
	Connector_Svc *svc = 0;
	svc_static_list_.get_used_svc(cid, svc);
	if (svc) {
		svc->close_fd();
		svc->reset();
		svc_static_list_.erase_svc(cid);
		svc_pool_.push(svc);
	}
	return 0;
}

int Connector::get_server_info(Server_Info &server_info) {
	server_info.svc_pool_free_list_size_ = svc_pool_.free_obj_list_size();
	server_info.svc_pool_used_list_size_ = svc_pool_.used_obj_list_size();
	server_info.svc_list_size_ = svc_static_list_.size();
	block_pool_group_.block_group_info(server_info.block_group_info_);
	return 0;
}

void Connector::free_cache(void) {
	block_pool_group_.shrink_all();
	svc_pool_.shrink_all();
}
