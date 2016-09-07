/*
 * V8_Manager.h
 *
 *  Created on: Jan 22, 2016
 *      Author: zhangyalei
 */

#ifndef V8_MANAGER_H_
#define V8_MANAGER_H_

#include <string.h>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include "Thread.h"

using namespace v8;

class Block_Buffer;
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
	V8_Manager(void);
	virtual ~V8_Manager(void);

	int init(const char *server_path);
	int fini(void);

private:
	static V8_Manager *instance_;
	Platform* platform_;
	Isolate* isolate_;
	Global<Context> context_;
};

///////////////////////////////////////////////////////////////////////////////
class Game_V8_Manager: public V8_Manager {
public:
	static Game_V8_Manager *instance(void);
	virtual void run_handler(void);

private:
	Game_V8_Manager(void);
	virtual ~Game_V8_Manager(void);
	Game_V8_Manager(const Game_V8_Manager &);
	const Game_V8_Manager &operator=(const Game_V8_Manager &);

private:
	static Game_V8_Manager *instance_;
};

#define GAME_V8_MANAGER Game_V8_Manager::instance()

//////////////////////////////////////////////////////////////////////////////////////
class Master_V8_Manager: public V8_Manager {
public:
	static Master_V8_Manager *instance(void);
	virtual void run_handler(void);

private:
	Master_V8_Manager(void);
	virtual ~Master_V8_Manager(void);
	Master_V8_Manager(const Master_V8_Manager &);
	const Master_V8_Manager &operator=(const Master_V8_Manager &);

private:
	static Master_V8_Manager *instance_;
};

#define MASTER_V8_MANAGER Master_V8_Manager::instance()

#endif /* V8_MANAGER_H_ */
