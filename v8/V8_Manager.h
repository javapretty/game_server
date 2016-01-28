/*
 * V8_Manager.h
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#ifndef V8_MANAGER_H_
#define V8_MANAGER_H_

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "V8_Context.h"
#include "Thread.h"

using namespace v8;
using namespace v8_wrap;

class V8_Manager: public Thread {
public:
	static V8_Manager *instance(void);
	void run_handler(void);
	int init(void);
	int fini(void);
	int start_v8(void);

	int wrap_block_buffer(void);

private:
	V8_Manager(void);
	virtual ~V8_Manager(void);
	V8_Manager(const V8_Manager &);
	const V8_Manager &operator=(const V8_Manager &);

private:
	static V8_Manager *instance_;
	Platform* platform_;
	context *context_;
};

#define V8_MANAGER V8_Manager::instance()

#endif /* V8_MANAGER_H_ */
