/*
 * Block_List.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef BLOCK_LIST_H_
#define BLOCK_LIST_H_

#include <sys/uio.h>
#include <limits.h>
#include <list>
#include "Block_Buffer.h"
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include "Lib_Log.h"

template <typename LOCK = NULL_MUTEX>
class Block_List {
public:
	typedef std::list<Block_Buffer *> BList;

	Block_List(void) : size_(0), max_size_(0) {
		list_.clear();
	}

	virtual ~Block_List(void) { }

	int push_back(Block_Buffer *buf) {
		int ret = -1;
		if (! buf) {
			LIB_LOG_ERROR("buf == 0");
			ret = -1;
		}

		//Mutex_Guard<LOCK> mon(this->lock_);
		GUARD(LOCK, mon, this->lock_);
		if (max_size_ > 0 &&  size_ > max_size_) {
			ret = -1;
		} else {
			++size_;
			list_.push_back(buf);
			ret = 0;
		}
		return ret;
	}

	Block_Buffer *front(void) {
		GUARD(LOCK, mon, this->lock_);
		if (list_.empty())
			return 0;
		else
			return list_.front();
	}

	Block_Buffer *pop_front(void) {
		GUARD(LOCK, mon, this->lock_);
		if (list_.empty()) {
			return 0;
		} else {
			Block_Buffer *buf = list_.front();
			list_.pop_front();
			--size_;
			return buf;
		}
	}

	bool empty(void) {
		GUARD(LOCK, mon, this->lock_);
		return list_.empty();
	}

	void clear(void) {
		GUARD(LOCK, mon, this->lock_);
		size_ = 0;
		list_.clear();
	}

	void swap(BList &blist) {
		GUARD(LOCK, mon, this->lock_);
		size_ = blist.size();
		list_.swap(blist);
	}

	size_t size(void) {
		GUARD(LOCK, mon, this->lock_);
		return size_;
	}

	int construct_iov(std::vector<iovec> &iov_vec, std::vector<Block_Buffer *> &iov_buff, size_t &sum_bytes) {
		GUARD(LOCK, mon, this->lock_);

		int iovcnt = size_ > IOV_MAX ? IOV_MAX : size_; 	/// IOV_MAX为XPG宏, 需要包含limits.h, 一般为1024, C中需加-D_XOPEN_SOURCE参数
		if (iovcnt == 0)
			return 0;

		struct iovec iov;
		int i = 0;
		sum_bytes = 0;
		for (BList::iterator it = list_.begin(); i < iovcnt && it != list_.end(); ++i, ++it) {
			iov.iov_base = (*it)->get_read_ptr();
			iov.iov_len = (*it)->readable_bytes();
			sum_bytes += (*it)->readable_bytes();
			iov_vec.push_back(iov);
			iov_buff.push_back(*it);
		}
		return iovcnt;
	}

	void pop_front(size_t num, size_t offset = 0) {
		GUARD(LOCK, mon, this->lock_);

		if (num > size_) {
			LIB_LOG_ERROR("num = %u, size_ = %u", num, size_);
			return;
		}

		size_t i = 0;
		for ( ; i < num; ++i) {
			list_.pop_front();
		}
		size_ -= i;

		if (offset > 0) {
			Block_Buffer *head = list_.front();
			if ((int)offset > head->get_write_idx()) {
				LIB_LOG_ERROR("offset = %ul, read_index = %ul, write_index = %ul.", offset, head->get_read_idx(), head->get_write_idx());
				return;
			}
			head->set_read_idx(head->get_read_idx() + offset);
		}
	}

	/// list_空间小于2, 或异常返回0; 合并成功返回空闲出来的buf指针.
	Block_Buffer *merge_first_second(void) {
		GUARD(LOCK, mon, this->lock_);

		if (size_ < 2)
			return 0;

		Block_Buffer *front_buf = list_.front();
		BList::iterator second_it = ++(list_.begin());
		Block_Buffer *second_buf = *second_it;
		list_.erase(second_it);
		--size_;

		second_buf->set_read_idx(second_buf->get_read_idx() + sizeof(int32_t)); /// 丢弃cid头

		if (second_buf->readable_bytes() <= 0) {
			LIB_LOG_ERROR("second_buf->readable_bytes() <= 0");
			second_buf->reset();
		} else {
			front_buf->copy(second_buf);
		}

		return second_buf;
	}

	void set_max_size(size_t max) {
		max_size_ = max;
	}

private:
	BList list_;
	size_t size_;
	size_t max_size_;
	LOCK lock_;
};

#endif /* BLOCK_LIST_H_ */
