/* cppsrc/main.cpp */
#include <napi.h>
#include "js-lib-vips.h"
#include "native_image.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  NativeImage::Init(env, exports);
  return js_lib_vips::InitLibVips(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
