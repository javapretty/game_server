/*
 * object_wrap.h
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#ifndef OBJECT_WRAP_H_
#define OBJECT_WRAP_H_

#include <assert.h>
#include <string>
#include "include/v8.h"

class MyClass
{
public:
	MyClass(int num);
	MyClass(const MyClass& rhs);
	virtual ~MyClass();

	int getNumber() const;
	void setNumber(int val);

	const char* getName() const;
	void setName(const char* name);

	void Method1();
	void Method2(const char* text);

private:
	int mNumber;
	std::string mName;
};

void myFunction();
void myFunction1(const char* text);
const char* myFunction2(int x);

class ObjectWrap : public MyClass
{
public:
	ObjectWrap(int num) : MyClass(num)
	{
		refs_ = 0;
	}

	virtual ~ObjectWrap() {
		if (handle_.IsEmpty())
			return;
		assert(handle_.IsNearDeath());
		handle_.ClearWeak();
		handle_.Reset();
	}

	inline void Wrap(v8::Local<v8::Object> handle) {
		assert(handle_.IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		handle->SetAlignedPointerInInternalField(0, this);
		handle_.Reset(v8::Isolate::GetCurrent(), handle);
		MakeWeak();
	}

	template <class T>
	static inline T* Unwrap(v8::Local<v8::Object> handle) {
		assert(!handle.IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		// Cast to ObjectWrap before casting to T.  A direct cast from void
		// to T won't work right when T has more than one base class.
		void* ptr = handle->GetAlignedPointerFromInternalField(0);
		ObjectWrap* wrap = static_cast<ObjectWrap*>(ptr);
		return static_cast<T*>(wrap);
	}

	inline v8::Local<v8::Object> handle() {
		return handle(v8::Isolate::GetCurrent());
	}

	inline v8::Local<v8::Object> handle(v8::Isolate* isolate) {
		return v8::Local<v8::Object>::New(isolate, handle_);
	}

	inline void MakeWeak(void) {
		handle_.SetWeak(this, WeakCallback);
		handle_.MarkIndependent();
	}

	/* Ref() marks the object as being attached to an event loop.
	* Refed objects will not be garbage collected, even if
	* all references are lost.
	*/
	virtual void Ref() {
		assert(!handle_.IsEmpty());
		handle_.ClearWeak();
		refs_++;
	}

	/* Unref() marks an object as detached from the event loop.  This is its
	* default state.  When an object with a "weak" reference changes from
	* attached to detached state it will be freed. Be careful not to access
	* the object after making this call as it might be gone!
	* (A "weak reference" means an object that only has a
	* persistent handle.)
	*
	* DO NOT CALL THIS FROM DESTRUCTOR
	*/
	virtual void Unref() {
		assert(!handle_.IsEmpty());
		assert(!handle_.IsWeak());
		assert(refs_ > 0);
		if (--refs_ == 0)
			MakeWeak();
	}

private:
	static void WeakCallback(const v8::WeakCallbackData<v8::Object, ObjectWrap>& data) {
		v8::Isolate* isolate = data.GetIsolate();
		v8::HandleScope scope(isolate);
		ObjectWrap* wrap = data.GetParameter();
		assert(wrap->refs_ == 0);
		assert(wrap->handle_.IsNearDeath());
		assert(
			data.GetValue() == v8::Local<v8::Object>::New(isolate, wrap->handle_));
		wrap->handle_.Reset();
		delete wrap;
	}

	v8::Persistent<v8::Object> handle_;
	int refs_;
};

#endif /* OBJECT_WRAP_H_ */
