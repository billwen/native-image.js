#include <napi.h>
#include <vips/vips8>
#include "js-lib-vips.h"

Napi::String js_lib_vips::Countdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "options missing").ThrowAsJavaScriptException();
    }

    Napi::Object options = info[0].As<Napi::Object>();
    js_lib_vips::CountdownOptions opts = ParseOptions(options);

    const std::string message = countdown(opts);
    Napi::String returnValue = Napi::String::New(env, message);

    return returnValue;
}

js_lib_vips::CountdownOptions js_lib_vips::ParseOptions(const Napi::Object& options) {
    js_lib_vips::CountdownOptions opts;

    Napi::Value outFilePath = options.Get("outFilePath");
    if (outFilePath.IsString()) {
        opts.outFilePath = outFilePath.As<Napi::String>().Utf8Value();
    } else {
        Napi::TypeError::New(options.Env(), "missing outFilePath").ThrowAsJavaScriptException();
    }

    Napi::Value width = options.Get("width");
    if (width.IsNumber()) {
        opts.width = width.As<Napi::Number>().Int32Value();
    } else {
        Napi::TypeError::New(options.Env(), "missing width").ThrowAsJavaScriptException();
    }

    Napi::Value height = options.Get("height");
    if (height.IsNumber()) {
        opts.height = height.As<Napi::Number>().Int32Value();
    } else {
        Napi::TypeError::New(options.Env(), "missing height").ThrowAsJavaScriptException();
    }

    Napi::Value bgColor = options.Get("bgColor");
    if (bgColor.IsString()) {
        opts.bgColor = bgColor.As<Napi::String>().Utf8Value();
    } else {
        Napi::TypeError::New(options.Env(), "missing bgColor").ThrowAsJavaScriptException();
    }

    return opts;
}

/**
 * Added Libvips interface
*/
Napi::Object js_lib_vips::InitLibVips(Napi::Env env, Napi::Object exports) {

    exports.Set("countdown", Napi::Function::New(env, js_lib_vips::Countdown));

    return exports;
}
