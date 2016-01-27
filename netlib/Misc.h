/*
 * Misc.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef MISC_H_
#define MISC_H_

#include "Block_Buffer.h"
#include <vector>

struct Block_Group_Info {
	size_t free_list_size_;
	size_t used_list_size_;
	size_t sum_bytes_;

	void serialize(Block_Buffer &buf) {
		buf.write_uint32(free_list_size_);
		buf.write_uint32(used_list_size_);
		buf.write_uint32(sum_bytes_);
	}

	void deserialize(Block_Buffer &buf) {
		uint32_t x = buf.read_uint32();
		free_list_size_ = x;
		x = buf.read_uint32();
		used_list_size_ = x;
		x = buf.read_uint32();
		sum_bytes_ = x;
	}

	void reset(void) {
		free_list_size_ = used_list_size_ = sum_bytes_ = 0;
	}
};

struct Server_Info {
	size_t svc_pool_free_list_size_;
	size_t svc_pool_used_list_size_;
	size_t svc_list_size_;
	std::vector<Block_Group_Info> block_group_info_;

	void serialize(Block_Buffer &buf) {
		buf.write_uint32(svc_pool_free_list_size_);
		buf.write_uint32(svc_pool_used_list_size_);
		buf.write_uint32(svc_list_size_);

		uint16_t s = block_group_info_.size();
		buf.write_uint16(s);

		for (uint16_t i = 0; i < s; ++i) {
			block_group_info_[i].serialize(buf);
		}
	}

	void deserialize(Block_Buffer &buf) {
		uint32_t x = buf.read_uint32();
		svc_pool_free_list_size_ = x;
		x = buf.read_uint32();
		svc_pool_used_list_size_ = x;
		x = buf.read_uint32();
		svc_list_size_ = x;

		uint16_t s = buf.read_uint16();
		Block_Group_Info info;
		for (uint16_t i = 0; i < s; ++i) {
			info.reset();
			info.deserialize(buf);
			block_group_info_.push_back(info);
		}
	}

	void reset(void) {
		svc_pool_free_list_size_ = svc_pool_used_list_size_ = svc_list_size_ = 0;
		block_group_info_.clear();
	}
};

class Misc {
public:
	Misc(void);
	virtual ~Misc(void);
};

#endif /* MISC_H_ */
