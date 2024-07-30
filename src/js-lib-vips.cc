#include <napi.h>
#include <vips/vips8>
#include "js-lib-vips.h"

Napi::String js_lib_vips::Countdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "options missing").ThrowAsJavaScriptException();
    }

    Napi::Object options = info[0].As<Napi::Object>();
    CountdownOptions opts = ParseOptions(options);

    const std::string message = countdown();
    Napi::String returnValue = Napi::String::New(env, message);

    return returnValue;
}

js_lib_vips::CountdownOptions js_lib_vips::ParseOptions(const Napi::Object& options) {
    js_lib_vips::CountdownOptions opts;

    Napi::Value message = options.Get("message");
    if (message.IsString()) {
        opts.bgColor = message.As<Napi::String>().Utf8Value();
    } else {
        Napi::TypeError::New(options.Env(), "message expected").ThrowAsJavaScriptException();
    }

    return opts;
}

/**
 * Added Libvips interface
*/
Napi::Object js_lib_vips::InitLibVips(Napi::Env env, Napi::Object exports) {

    if (VIPS_INIT ("js-lib-vips")) 
        vips_error_exit (NULL);

    exports.Set("countdown", Napi::Function::New(env, js_lib_vips::Countdown));

    return exports;
}
