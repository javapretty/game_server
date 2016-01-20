/*
 * V8_Process.h
 *
 *  Created on: 2016年1月20日
 *      Author: zhangyalei
 */

#ifndef V8_PROCESS_H_
#define V8_PROCESS_H_

#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include "include/v8.h"
#include "include/libplatform/libplatform.h"

using namespace std;
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


// These interfaces represent an existing request processing interface.
// The idea is to imagine a real application that uses these interfaces
// and then add scripting capabilities that allow you to interact with
// the objects through JavaScript.

/**
 * A simplified http request.
 */
class HttpRequest {
 public:
  virtual ~HttpRequest() { }
  virtual const string& Path() = 0;
  virtual const string& Referrer() = 0;
  virtual const string& Host() = 0;
  virtual const string& UserAgent() = 0;
};


/**
 * The abstract superclass of http request processors.
 */
class HttpRequestProcessor {
 public:
  virtual ~HttpRequestProcessor() { }

  // Initialize this processor.  The map contains options that control
  // how requests should be processed.
  virtual bool Initialize(map<string, string>* options,
                          map<string, string>* output) = 0;

  // Process a single request.
  virtual bool Process(HttpRequest* req) = 0;

  static void Log(const char* event);
};


/**
 * An http request processor that is scriptable using JavaScript.
 */
class JsHttpRequestProcessor : public HttpRequestProcessor {
 public:
  // Creates a new processor that processes requests by invoking the
  // Process function of the JavaScript script given as an argument.
  JsHttpRequestProcessor(Isolate* isolate, Local<String> script)
      : isolate_(isolate), script_(script) {}
  virtual ~JsHttpRequestProcessor();

  virtual bool Initialize(map<string, string>* opts,
                          map<string, string>* output);
  virtual bool Process(HttpRequest* req);

 private:
  // Execute the script associated with this processor and extract the
  // Process function.  Returns true if this succeeded, otherwise false.
  bool ExecuteScript(Local<String> script);

  // Wrap the options and output map in a JavaScript objects and
  // install it in the global namespace as 'options' and 'output'.
  bool InstallMaps(map<string, string>* opts, map<string, string>* output);

  // Constructs the template that describes the JavaScript wrapper
  // type for requests.
  static Local<ObjectTemplate> MakeRequestTemplate(Isolate* isolate);
  static Local<ObjectTemplate> MakeMapTemplate(Isolate* isolate);

  // Callbacks that access the individual fields of request objects.
  static void GetPath(Local<String> name,
                      const PropertyCallbackInfo<Value>& info);
  static void GetReferrer(Local<String> name,
                          const PropertyCallbackInfo<Value>& info);
  static void GetHost(Local<String> name,
                      const PropertyCallbackInfo<Value>& info);
  static void GetUserAgent(Local<String> name,
                           const PropertyCallbackInfo<Value>& info);

  // Callbacks that access maps
  static void MapGet(Local<Name> name, const PropertyCallbackInfo<Value>& info);
  static void MapSet(Local<Name> name, Local<Value> value,
                     const PropertyCallbackInfo<Value>& info);

  // Utility methods for wrapping C++ objects as JavaScript objects,
  // and going back again.
  Local<Object> WrapMap(map<string, string>* obj);
  static map<string, string>* UnwrapMap(Local<Object> obj);
  Local<Object> WrapRequest(HttpRequest* obj);
  static HttpRequest* UnwrapRequest(Local<Object> obj);

  Isolate* GetIsolate() { return isolate_; }

  Isolate* isolate_;
  Local<String> script_;
  Global<Context> context_;
  Global<Function> process_;
  static Global<ObjectTemplate> request_template_;
  static Global<ObjectTemplate> map_template_;
};

/**
 * A simplified http request.
 */
class StringHttpRequest : public HttpRequest {
 public:
  StringHttpRequest(const string& path,
                    const string& referrer,
                    const string& host,
                    const string& user_agent);
  virtual const string& Path() { return path_; }
  virtual const string& Referrer() { return referrer_; }
  virtual const string& Host() { return host_; }
  virtual const string& UserAgent() { return user_agent_; }
 private:
  string path_;
  string referrer_;
  string host_;
  string user_agent_;
};

#endif /* V8_PROCESS_H_ */
