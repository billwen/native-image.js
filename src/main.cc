/* cppsrc/main.cpp */
#include <napi.h>
#include "native_image.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  if (VIPS_INIT ("js-lib-vips")) 
    vips_error_exit (NULL);

  return NativeImage::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)
