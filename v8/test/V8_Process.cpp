/*
 * process.cpp
 *
 *  Created on: 2016年1月19日
 *      Author: zhangyalei
 */

#include "V8_Wrap.h"
#include "V8_Process.h"

static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1) return;
  HandleScope scope(args.GetIsolate());
  Local<Value> arg = args[0];
  String::Utf8Value value(arg);
  HttpRequestProcessor::Log(*value);
}

// Execute the script and fetch the Process method.
bool JsHttpRequestProcessor::Initialize(map<string, string>* opts,
                                        map<string, string>* output) {
  // Create a handle scope to hold the temporary references.
  HandleScope handle_scope(GetIsolate());

  // Create a template for the global object where we set the
  // built-in global functions.
  Local<ObjectTemplate> global = ObjectTemplate::New(GetIsolate());
  global->Set(String::NewFromUtf8(GetIsolate(), "log", NewStringType::kNormal)
                  .ToLocalChecked(),
              FunctionTemplate::New(GetIsolate(), LogCallback));

  // Each processor gets its own context so different processors don't
  // affect each other. Context::New returns a persistent handle which
  // is what we need for the reference to remain after we return from
  // this method. That persistent handle has to be disposed in the
  // destructor.
  v8::Local<v8::Context> context = Context::New(GetIsolate(), NULL, global);
  context_.Reset(GetIsolate(), context);

  // Enter the new context so all the following operations take place
  // within it.
  Context::Scope context_scope(context);

  // Make the options mapping available within the context
  if (!InstallMaps(opts, output))
    return false;

  // Compile and run the script
  if (!ExecuteScript(script_))
    return false;

  // The script compiled and ran correctly.  Now we fetch out the
  // Process function from the global object.
  Local<String> process_name =
      String::NewFromUtf8(GetIsolate(), "Process", NewStringType::kNormal)
          .ToLocalChecked();
  Local<Value> process_val;
  // If there is no Process function, or if it is not a function,
  // bail out
  if (!context->Global()->Get(context, process_name).ToLocal(&process_val) ||
      !process_val->IsFunction()) {
    return false;
  }

  // It is a function; cast it to a Function
  Local<Function> process_fun = Local<Function>::Cast(process_val);

  // Store the function in a Global handle, since we also want
  // that to remain after this call returns
  process_.Reset(GetIsolate(), process_fun);

  // All done; all went well
  return true;
}

bool JsHttpRequestProcessor::ExecuteScript(Local<String> script) {
  HandleScope handle_scope(GetIsolate());

  // We're just about to compile the script; set up an error handler to
  // catch any exceptions the script might throw.
  TryCatch try_catch(GetIsolate());

  Local<Context> context(GetIsolate()->GetCurrentContext());

  // Compile the script and check for errors.
  Local<Script> compiled_script;
  if (!Script::Compile(context, script).ToLocal(&compiled_script)) {
    String::Utf8Value error(try_catch.Exception());
    Log(*error);
    // The script failed to compile; bail out.
    return false;
  }

  // Run the script!
  Local<Value> result;
  if (!compiled_script->Run(context).ToLocal(&result)) {
    // The TryCatch above is still in effect and will have caught the error.
    String::Utf8Value error(try_catch.Exception());
    Log(*error);
    // Running the script failed; bail out.
    return false;
  }
  return true;
}

bool JsHttpRequestProcessor::InstallMaps(map<string, string>* opts,
                                         map<string, string>* output) {
  HandleScope handle_scope(GetIsolate());

  // Wrap the map object in a JavaScript wrapper
  Local<Object> opts_obj = WrapMap(opts);

  v8::Local<v8::Context> context =
      v8::Local<v8::Context>::New(GetIsolate(), context_);

  // Set the options object as a property on the global object.
  context->Global()
      ->Set(context,
            String::NewFromUtf8(GetIsolate(), "options", NewStringType::kNormal)
                .ToLocalChecked(),
            opts_obj)
      .FromJust();

  Local<Object> output_obj = WrapMap(output);
  context->Global()
      ->Set(context,
            String::NewFromUtf8(GetIsolate(), "output", NewStringType::kNormal)
                .ToLocalChecked(),
            output_obj)
      .FromJust();

  return true;
}

bool JsHttpRequestProcessor::Process(HttpRequest* request) {
  // Create a handle scope to keep the temporary object references.
  HandleScope handle_scope(GetIsolate());

  v8::Local<v8::Context> context =
      v8::Local<v8::Context>::New(GetIsolate(), context_);

  // Enter this processor's context so all the remaining operations
  // take place there
  Context::Scope context_scope(context);

  // Wrap the C++ request object in a JavaScript wrapper
  Local<Object> request_obj = WrapRequest(request);

  // Set up an exception handler before calling the Process function
  TryCatch try_catch(GetIsolate());

  // Invoke the process function, giving the global object as 'this'
  // and one argument, the request.
  const int argc = 1;
  Local<Value> argv[argc] = {request_obj};
  v8::Local<v8::Function> process =
      v8::Local<v8::Function>::New(GetIsolate(), process_);
  Local<Value> result;
  if (!process->Call(context, context->Global(), argc, argv).ToLocal(&result)) {
    String::Utf8Value error(try_catch.Exception());
    Log(*error);
    return false;
  } else {
    return true;
  }
}

JsHttpRequestProcessor::~JsHttpRequestProcessor() {
  // Dispose the persistent handles.  When noone else has any
  // references to the objects stored in the handles they will be
  // automatically reclaimed.
  context_.Reset();
  process_.Reset();
}

Global<ObjectTemplate> JsHttpRequestProcessor::request_template_;
Global<ObjectTemplate> JsHttpRequestProcessor::map_template_;


// Utility function that wraps a C++ http request object in a
// JavaScript object.
Local<Object> JsHttpRequestProcessor::WrapMap(map<string, string>* obj) {
  // Local scope for temporary handles.
  EscapableHandleScope handle_scope(GetIsolate());

  // Fetch the template for creating JavaScript map wrappers.
  // It only has to be created once, which we do on demand.
  if (map_template_.IsEmpty()) {
    Local<ObjectTemplate> raw_template = MakeMapTemplate(GetIsolate());
    map_template_.Reset(GetIsolate(), raw_template);
  }
  Local<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(GetIsolate(), map_template_);

  // Create an empty map wrapper.
  Local<Object> result =
      templ->NewInstance(GetIsolate()->GetCurrentContext()).ToLocalChecked();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Local<External> map_ptr = External::New(GetIsolate(), obj);

  // Store the map pointer in the JavaScript wrapper.
  result->SetInternalField(0, map_ptr);

  // Return the result through the current handle scope.  Since each
  // of these handles will go away when the handle scope is deleted
  // we need to call Close to let one, the result, escape into the
  // outer handle scope.
  return handle_scope.Escape(result);
}

// Utility function that extracts the C++ map pointer from a wrapper
// object.
map<string, string>* JsHttpRequestProcessor::UnwrapMap(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<map<string, string>*>(ptr);
}

// Convert a JavaScript string to a std::string.  To not bother too
// much with string encodings we just use ascii.
string ObjectToString(Local<Value> value) {
  String::Utf8Value utf8_value(value);
  return string(*utf8_value);
}

void JsHttpRequestProcessor::MapGet(Local<Name> name,
                                    const PropertyCallbackInfo<Value>& info) {
  if (name->IsSymbol()) return;

  // Fetch the map wrapped by this object.
  map<string, string>* obj = UnwrapMap(info.Holder());

  // Convert the JavaScript string to a std::string.
  string key = ObjectToString(Local<String>::Cast(name));

  // Look up the value if it exists using the standard STL ideom.
  map<string, string>::iterator iter = obj->find(key);

  // If the key is not present return an empty handle as signal
  if (iter == obj->end()) return;

  // Otherwise fetch the value and wrap it in a JavaScript string
  const string& value = (*iter).second;
  info.GetReturnValue().Set(
      String::NewFromUtf8(info.GetIsolate(), value.c_str(),
                          NewStringType::kNormal,
                          static_cast<int>(value.length())).ToLocalChecked());
}

void JsHttpRequestProcessor::MapSet(Local<Name> name, Local<Value> value_obj,
                                    const PropertyCallbackInfo<Value>& info) {
  if (name->IsSymbol()) return;

  // Fetch the map wrapped by this object.
  map<string, string>* obj = UnwrapMap(info.Holder());

  // Convert the key and value to std::strings.
  string key = ObjectToString(Local<String>::Cast(name));
  string value = ObjectToString(value_obj);

  // Update the map.
  (*obj)[key] = value;

  // Return the value; any non-empty handle will work.
  info.GetReturnValue().Set(value_obj);
}


Local<ObjectTemplate> JsHttpRequestProcessor::MakeMapTemplate(
    Isolate* isolate) {
  EscapableHandleScope handle_scope(isolate);

  Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
  result->SetInternalFieldCount(1);
  result->SetHandler(NamedPropertyHandlerConfiguration(MapGet, MapSet));

  // Again, return the result through the current handle scope.
  return handle_scope.Escape(result);
}

/**
 * Utility function that wraps a C++ http request object in a
 * JavaScript object.
 */
Local<Object> JsHttpRequestProcessor::WrapRequest(HttpRequest* request) {
  // Local scope for temporary handles.
  EscapableHandleScope handle_scope(GetIsolate());

  // Fetch the template for creating JavaScript http request wrappers.
  // It only has to be created once, which we do on demand.
  if (request_template_.IsEmpty()) {
    Local<ObjectTemplate> raw_template = MakeRequestTemplate(GetIsolate());
    request_template_.Reset(GetIsolate(), raw_template);
  }
  Local<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(GetIsolate(), request_template_);

  // Create an empty http request wrapper.
  Local<Object> result =
      templ->NewInstance(GetIsolate()->GetCurrentContext()).ToLocalChecked();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Local<External> request_ptr = External::New(GetIsolate(), request);

  // Store the request pointer in the JavaScript wrapper.
  result->SetInternalField(0, request_ptr);

  // Return the result through the current handle scope.  Since each
  // of these handles will go away when the handle scope is deleted
  // we need to call Close to let one, the result, escape into the
  // outer handle scope.
  return handle_scope.Escape(result);
}

/**
 * Utility function that extracts the C++ http request object from a
 * wrapper object.
 */
HttpRequest* JsHttpRequestProcessor::UnwrapRequest(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<HttpRequest*>(ptr);
}

void JsHttpRequestProcessor::GetPath(Local<String> name,
                                     const PropertyCallbackInfo<Value>& info) {
  // Extract the C++ request object from the JavaScript wrapper.
  HttpRequest* request = UnwrapRequest(info.Holder());

  // Fetch the path.
  const string& path = request->Path();

  // Wrap the result in a JavaScript string and return it.
  info.GetReturnValue().Set(
      String::NewFromUtf8(info.GetIsolate(), path.c_str(),
                          NewStringType::kNormal,
                          static_cast<int>(path.length())).ToLocalChecked());
}

void JsHttpRequestProcessor::GetReferrer(
    Local<String> name,
    const PropertyCallbackInfo<Value>& info) {
  HttpRequest* request = UnwrapRequest(info.Holder());
  const string& path = request->Referrer();
  info.GetReturnValue().Set(
      String::NewFromUtf8(info.GetIsolate(), path.c_str(),
                          NewStringType::kNormal,
                          static_cast<int>(path.length())).ToLocalChecked());
}

void JsHttpRequestProcessor::GetHost(Local<String> name,
                                     const PropertyCallbackInfo<Value>& info) {
  HttpRequest* request = UnwrapRequest(info.Holder());
  const string& path = request->Host();
  info.GetReturnValue().Set(
      String::NewFromUtf8(info.GetIsolate(), path.c_str(),
                          NewStringType::kNormal,
                          static_cast<int>(path.length())).ToLocalChecked());
}

void JsHttpRequestProcessor::GetUserAgent(
    Local<String> name,
    const PropertyCallbackInfo<Value>& info) {
  HttpRequest* request = UnwrapRequest(info.Holder());
  const string& path = request->UserAgent();
  info.GetReturnValue().Set(
      String::NewFromUtf8(info.GetIsolate(), path.c_str(),
                          NewStringType::kNormal,
                          static_cast<int>(path.length())).ToLocalChecked());
}

Local<ObjectTemplate> JsHttpRequestProcessor::MakeRequestTemplate(
    Isolate* isolate) {
  EscapableHandleScope handle_scope(isolate);

  Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
  result->SetInternalFieldCount(1);

  // Add accessors for each of the fields of the request.
  result->SetAccessor(
      String::NewFromUtf8(isolate, "path", NewStringType::kInternalized)
          .ToLocalChecked(),
      GetPath);
  result->SetAccessor(
      String::NewFromUtf8(isolate, "referrer", NewStringType::kInternalized)
          .ToLocalChecked(),
      GetReferrer);
  result->SetAccessor(
      String::NewFromUtf8(isolate, "host", NewStringType::kInternalized)
          .ToLocalChecked(),
      GetHost);
  result->SetAccessor(
      String::NewFromUtf8(isolate, "userAgent", NewStringType::kInternalized)
          .ToLocalChecked(),
      GetUserAgent);

  // Again, return the result through the current handle scope.
  return handle_scope.Escape(result);
}

void HttpRequestProcessor::Log(const char* event) {
  printf("Logged: %s\n", event);
}

StringHttpRequest::StringHttpRequest(const string& path,
                                     const string& referrer,
                                     const string& host,
                                     const string& user_agent)
    : path_(path),
      referrer_(referrer),
      host_(host),
      user_agent_(user_agent) { }


void ParseOptions(int argc,
                  char* argv[],
                  map<string, string>* options,
                  string* file) {
  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    size_t index = arg.find('=', 0);
    if (index == string::npos) {
      *file = arg;
    } else {
      string key = arg.substr(0, index);
      string value = arg.substr(index+1);
      (*options)[key] = value;
    }
  }
}

const int kSampleSize = 6;
StringHttpRequest kSampleRequests[kSampleSize] = {
  StringHttpRequest("/process.cc", "localhost", "google.com", "firefox"),
  StringHttpRequest("/", "localhost", "google.net", "firefox"),
  StringHttpRequest("/", "localhost", "google.org", "safari"),
  StringHttpRequest("/", "localhost", "yahoo.com", "ie"),
  StringHttpRequest("/", "localhost", "yahoo.com", "safari"),
  StringHttpRequest("/", "localhost", "yahoo.com", "firefox")
};

bool ProcessEntries(v8::Platform* platform, HttpRequestProcessor* processor,
                    int count, StringHttpRequest* reqs) {
  for (int i = 0; i < count; i++) {
    bool result = processor->Process(&reqs[i]);
    while (v8::platform::PumpMessageLoop(platform, Isolate::GetCurrent()))
      continue;
    if (!result) return false;
  }
  return true;
}

void PrintMap(map<string, string>* m) {
  for (map<string, string>::iterator i = m->begin(); i != m->end(); i++) {
    pair<string, string> entry = *i;
    printf("%s: %s\n", entry.first.c_str(), entry.second.c_str());
  }
}

int process(int argc, char* argv[]) {
  v8::V8::InitializeICU();
  v8::V8::InitializeExternalStartupData(argv[0]);
  v8::Platform* platform = v8::platform::CreateDefaultPlatform();
  v8::V8::InitializePlatform(platform);
  v8::V8::Initialize();
  map<string, string> options;
  string file;
  ParseOptions(argc, argv, &options, &file);
  if (file.empty()) {
    fprintf(stderr, "No script was specified.\n");
    return 1;
  }
  ArrayBufferAllocator array_buffer_allocator;
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator = &array_buffer_allocator;
  Isolate* isolate = Isolate::New(create_params);
  Isolate::Scope isolate_scope(isolate);
  HandleScope scope(isolate);
  Local<String> source;
  if (!ReadFile(isolate, file.c_str()).ToLocal(&source)) {
    fprintf(stderr, "Error reading '%s'.\n", file.c_str());
    return 1;
  }
  JsHttpRequestProcessor processor(isolate, source);
  map<string, string> output;
  if (!processor.Initialize(&options, &output)) {
    fprintf(stderr, "Error initializing processor.\n");
    return 1;
  }
  if (!ProcessEntries(platform, &processor, kSampleSize, kSampleRequests))
    return 1;
  PrintMap(&output);

  return 0;
}
