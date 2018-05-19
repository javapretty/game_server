/*
 * Public_Define.cpp
 *
 *  Created on: Aug 6,2016
 *      Author: zhangyalei
 */

#include "Block_Buffer.h"
#include "Public_Define.h"

void Block_Group_Info::serialize(Block_Buffer &buf) {
	buf.write_int32(free_list_size_);
	buf.write_int32(used_list_size_);
	buf.write_int32(sum_bytes_);
}

void Block_Group_Info::deserialize(Block_Buffer &buf) {
	buf.read_int32(free_list_size_);
	buf.read_int32(used_list_size_);
	buf.read_int32(sum_bytes_);
}

void Block_Group_Info::reset(void) {
	free_list_size_ = 0;
	used_list_size_ = 0;
	sum_bytes_ = 0;
}

void Server_Info::serialize(Block_Buffer &buf) {
	buf.write_int32(svc_pool_free_list_size_);
	buf.write_int32(svc_pool_used_list_size_);
	buf.write_int32(svc_list_size_);

	uint16_t s = block_group_info_.size();
	buf.write_uint16(s);
	for (uint16_t i = 0; i < s; ++i) {
		block_group_info_[i].serialize(buf);
	}
}

void Server_Info::deserialize(Block_Buffer &buf) {
	buf.read_int32(svc_pool_free_list_size_);
	buf.read_int32(svc_pool_used_list_size_);
	buf.read_int32(svc_list_size_);

	uint16_t count = 0;
	buf.read_uint16(count);
	Block_Group_Info info;
	for (uint16_t i = 0; i < count; ++i) {
		info.reset();
		info.deserialize(buf);
		block_group_info_.push_back(info);
	}
}

void Server_Info::reset(void) {
	svc_pool_free_list_size_ = 0;
	svc_pool_used_list_size_ = 0;
	svc_list_size_ = 0;
	block_group_info_.clear();
}
