/* cppsrc/main.cpp */
#include <napi.h>
#include "hello.h"
#include "actual_class.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  js_vips::Init(env, exports);
  return ActualClassWrapped::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
