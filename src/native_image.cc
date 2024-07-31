#include "native_image.h"

Napi::FunctionReference NativeImage::constructor;

Napi::Object NativeImage::CreateEmpty(Napi::Env env, Napi::Value arg) {
    Napi::EscapableHandleScope scope(env);
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    return constructor.New({});
}

Napi::Object NativeImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NativeImage", {
        InstanceMethod("countdown", &NativeImage::Countdown),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("NativeImage", func);
    return exports;
}

NativeImage::NativeImage(const Napi::CallbackInfo& info): Napi::ObjectWrap<NativeImage>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    this->image_ = new vips::VImage();
}

/**
 * Generate a countdown gif image
 */
Napi::Value NativeImage::Countdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    return Napi::String::New(env, "Hello NativeImage");
}
