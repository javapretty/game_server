/*
 * Server.cpp
 *
 *  Created on: Jan 16,2016
 *      Author: zhangyalei
 */

#include "Server.h"

int Server_Accept::accept_svc(int connfd) {
	Server_Svc *svc = server_->svc_pool().pop();
	if (! svc) {
		LIB_LOG_ERROR("svc == NULL");
		return -1;
	}

	int cid = server_->svc_list().record_svc(svc);
	if (cid == -1) {
		LIB_LOG_ERROR("cid == -1");
		server_->svc_pool().push(svc);
		return -1;
	}
	LIB_LOG_DEBUG("connfd=%d cid:%d", connfd, cid);

	svc->reset();
	svc->create_handler(server_->network_protocol_type());
	svc->set_cid(cid);
	svc->set_fd(connfd);
	svc->set_peer_addr();
	svc->set_server(server_);

	svc->register_recv_handler();
	svc->register_send_handler();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

int Server_Receive::drop_handler(int cid) {
	return server_->send().push_drop(cid);
}

Svc *Server_Receive::find_svc(int cid) {
	return server_->find_svc(cid);
}

////////////////////////////////////////////////////////////////////////////////

Block_Buffer *Server_Send::pop_block(int cid) {
	return server_->pop_block(cid);
}

int Server_Send::push_block(int cid, Block_Buffer *buffer) {
	return server_->push_block(cid, buffer);
}

int Server_Send::drop_handler(int cid) {
	LIB_LOG_INFO("server drop_handler, cid = %d", cid);
	server_->drop_cid_list().push_back(cid);
	return server_->recycle_svc(cid);
}

Svc *Server_Send::find_svc(int cid) {
	return server_->find_svc(cid);
};

////////////////////////////////////////////////////////////////////////////////

Block_Buffer *Server_Svc::pop_block(int cid) {
	return server_->pop_block(cid);
}

int Server_Svc::push_block(int cid, Block_Buffer *buffer) {
	return server_->push_block(cid, buffer);
}

int Server_Svc::post_block(Block_Buffer* buffer) {
	return server_->block_list().push_back(buffer);
}

int Server_Svc::register_recv_handler(void) {
	if (! get_reg_recv()) {
		server_->receive().register_svc(this);
		set_reg_recv(true);
	}
	return 0;
}

int Server_Svc::unregister_recv_handler(void) {
	if (get_reg_recv()) {
		server_->receive().unregister_svc(this);
		set_reg_recv(false);
	}
	return 0;
}

int Server_Svc::register_send_handler(void) {
	if (! get_reg_send()) {
		server_->send().register_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Server_Svc::unregister_send_handler(void) {
	if (get_reg_send()) {
		server_->send().unregister_svc(this);
		set_reg_send(true);
	}
	return 0;
}

int Server_Svc::close_handler(int cid) {
	server_->receive().push_drop(cid);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

Server::Server(void): network_protocol_type_(TCP) { }

Server::~Server(void) { }

void Server::run_handler(void) {
	process_list();
}

void Server::process_list(void) {
	LIB_LOG_TRACE("SHOULD NOT HERE");
}

void Server::set(int port, Time_Value &recv_timeout, Time_Value &send_interval, int protocol_type) {
	accept_.set(this, port);
	receive_.set(this, 0, &recv_timeout);
	send_.set(this, 0, send_interval);
	network_protocol_type_ = (NetWork_Protocol)protocol_type;
}

int Server::init(void) {
	accept_.init();
	receive_.init();
	send_.init();
	return 0;
}

int Server::start(void) {
	accept_.thr_create();
	receive_.thr_create();
	send_.thr_create();
	return 0;
}

Block_Buffer *Server::pop_block(int cid) {
	return block_pool_group_.pop_block(cid);
}

int Server::push_block(int cid, Block_Buffer *buffer) {
	return block_pool_group_.push_block(cid, buffer);
}

int Server::send_block(int cid, Block_Buffer &buffer) {
	return send_.push_svc_block(cid, buffer);
}

Svc *Server::find_svc(int cid) {
	Server_Svc *svc = 0;
	svc_static_list_.get_used_svc(cid, svc);
	return svc;
}

int Server::recycle_svc(int cid) {
	Server_Svc *svc = 0;
	svc_static_list_.get_used_svc(cid, svc);
	if (svc) {
		svc->close_fd();
		svc->reset();
		svc_static_list_.erase_svc(cid);
		svc_pool_.push(svc);
	}
	return 0;
}

int Server::get_server_info(Server_Info &server_info) {
	server_info.svc_pool_free_list_size_ = svc_pool_.free_obj_list_size();
	server_info.svc_pool_used_list_size_ = svc_pool_.used_obj_list_size();
	server_info.svc_list_size_ = svc_static_list_.size();
	block_pool_group_.block_group_info(server_info.block_group_info_);
	return 0;
}

void Server::free_cache(void) {
	block_pool_group_.shrink_all();
	svc_pool_.shrink_all();
}
