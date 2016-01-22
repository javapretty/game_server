/*
 * V8_Manager.h
 *
 *  Created on: 2016年1月22日
 *      Author: zhangyalei
 */

#ifndef V8_MANAGER_H_
#define V8_MANAGER_H_

#include "Thread.h"
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

using namespace v8;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
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
	int start_v8(void);
	int process_script(void);

private:
	V8_Manager(void);
	virtual ~V8_Manager(void);
	V8_Manager(const V8_Manager &);
	const V8_Manager &operator=(const V8_Manager &);

private:
	static V8_Manager *instance_;
	Isolate* isolate_;
	Platform* platform_;
};

#define V8_MANAGER V8_Manager::instance()

#endif /* V8_MANAGER_H_ */
