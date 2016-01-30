/*
 * V8_Manager.h
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#ifndef V8_MANAGER_H_
#define V8_MANAGER_H_

#include <string.h>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "V8_Context.h"
#include "Thread.h"

using namespace v8;
using namespace v8_wrap;

class ArrayBufferAllocator : public ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

class V8_Manager: public Thread {
public:
	static V8_Manager *instance(void);
	void run_handler(void);
	int init(void);
	int fini(void);
	int start_v8_base(void);
	int start_v8_context(void);

	int wrap_block(void);
	int wrap_pointer();

private:
	V8_Manager(void);
	virtual ~V8_Manager(void);
	V8_Manager(const V8_Manager &);
	const V8_Manager &operator=(const V8_Manager &);

private:
	static V8_Manager *instance_;
	Platform* platform_;
	Isolate* isolate_;
	Global<Context> global_context_;

	context *context_;
};

#define V8_MANAGER V8_Manager::instance()

#endif /* V8_MANAGER_H_ */
