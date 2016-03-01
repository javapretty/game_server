/*
 * Svc_Static_List.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 *
 * 		静态链表
 *
 */

#ifndef SVC_STATIC_LIST_H_
#define SVC_STATIC_LIST_H_

#include <vector>
#include "Thread_Mutex.h"
#include "Mutex_Guard.h"
#include "Time_Value.h"
#include "Lib_Log.h"

template <typename NODE_TYPE>
class Svc_Static_List_Node {
public:
	const static int null_ptr = -1;

	Svc_Static_List_Node(void)
	: idx_(null_ptr),
	  prev_(null_ptr),
	  next_(null_ptr),
	  erase_timestamp_(Time_Value::zero),
	  is_used_(false)
	{ }

	void reset(void) {
		idx_ = null_ptr;
		prev_ = next_ = null_ptr;
		erase_timestamp_ = Time_Value::zero;
		is_used_ = false;
	}

	int idx_;
	int prev_, next_;
	Time_Value erase_timestamp_;
	bool is_used_;

	NODE_TYPE node_;
};

template <typename NODE_TYPE, typename LOCK = NULL_MUTEX>
class Svc_Static_List {
public:
	typedef Svc_Static_List_Node<NODE_TYPE> Node;

	const static int free_list_head_idx = 0;	/// 空闲链头下标
	const static int used_list_head_idx = 1;	/// 已使用链头下标

	Svc_Static_List(void) {
		this->init();
	}

	virtual ~Svc_Static_List(void) {
		this->fini();
	}

	/// 记录在已使用链头, 并返回下标作为cid
	int record_svc(NODE_TYPE pnode) {
		GUARD(LOCK, mon, lock_);

		int idx = -1;

		if (free_head().prev_ == free_list_head_idx && free_head().next_ == free_list_head_idx) { /// 空闲链空
			Node node;

			node.idx_ = data_list_.size();
			node.prev_ = used_list_head_idx;
			node.next_ = used_head().next_;
			node.is_used_ = true;
			node.node_ = pnode;

			data_list_[used_head().next_].prev_ = node.idx_;
			used_head().next_ = node.idx_;

			data_list_.push_back(node);

			idx = node.idx_;
		} else {
			Node &first_free_node = data_list_[free_head().next_];
			if (Time_Value::gettimeofday() - first_free_node.erase_timestamp_ > erase_interval_) { /// 判断第一个空闲节点是否超过“冷却”时间
				data_list_[first_free_node.next_].prev_ = first_free_node.prev_;
				free_head().next_ = first_free_node.next_;

				data_list_[used_head().next_].prev_ = first_free_node.idx_;

				first_free_node.next_ = used_head().next_;
				first_free_node.prev_ = used_list_head_idx;

				used_head().next_ = first_free_node.idx_;

				first_free_node.is_used_ = true;
				first_free_node.node_ = pnode;

				idx = first_free_node.idx_;
			} else {
				Node node;
				node.idx_ = data_list_.size();
				node.prev_ = used_list_head_idx;
				node.next_ = used_head().next_;
				node.is_used_ = true;
				node.node_ = pnode;

				data_list_[used_head().next_].prev_ = node.idx_;
				used_head().next_ = node.idx_;

				data_list_.push_back(node);

				idx = node.idx_;
			}
		}

		return idx;
	}

	/// erase掉一个在 已使用链 中的节点, 将其放在 空闲链 尾
	int erase_svc(int idx) {
		GUARD(LOCK, mon, lock_);

		if (! validate_cid(idx)) {
			LIB_LOG_TRACE("cid = %d", idx);
			return -1;
		}

		Node &node = data_list_[idx];

		if (! node.is_used_) {
			LIB_LOG_TRACE("idx = %d is a free node", idx);
			return -1;
		}

		node.erase_timestamp_ = Time_Value::gettimeofday();

		/// 从已使用链中删除
		data_list_[node.next_].prev_ = node.prev_;
		data_list_[node.prev_].next_ = node.next_;

		/// 插入空闲链尾
		node.prev_ = free_head().prev_;
		node.next_ = free_list_head_idx;

		data_list_[free_head().prev_].next_ = node.idx_;
		free_head().prev_ = node.idx_;

		node.is_used_ = false;

		return 0;
	}

	bool get_used_svc(int idx, NODE_TYPE &node_val) {
		GUARD(LOCK, mon, lock_);

		if (! validate_cid(idx)) {
			LIB_LOG_TRACE("cid = %d", idx);
			return false;
		}

		Node &node = data_list_[idx];
		if (! node.is_used_) {
			return false;
		}

		node_val = node.node_;

		return true;
	}

	size_t size(void) {
		GUARD(LOCK, mon, lock_);
		return data_list_.size();
	}


private:
	Node &free_head(void) {
		return data_list_[free_list_head_idx];
	}

	Node &used_head(void) {
		return data_list_[used_list_head_idx];
	}

	bool validate_cid(int cid) {
		if ((size_t)cid < 2 || (size_t)cid >= data_list_.size())
			return false;
		else
			return true;
	}

	void init(void) {
		erase_interval_ = Time_Value(12 * 60, 0);

		data_list_.clear();

		Node node;

		/// 空闲链头
		node.reset();
		node.idx_ = free_list_head_idx;
		node.prev_ = node.next_ = free_list_head_idx;
		node.is_used_ = true;
		data_list_.push_back(node);

		/// 已使用链头
		node.reset();
		node.idx_ = used_list_head_idx;
		node.prev_ = node.next_ = used_list_head_idx;
		node.is_used_ = true;
		data_list_.push_back(node);
	}

	void fini(void) {
		data_list_.clear();
	}

private:
	Time_Value erase_interval_;
	std::vector<Node> data_list_;
	LOCK lock_;
};

#endif /* SVC_STATIC_LIST_H_ */
