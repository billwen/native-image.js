#include "hello.h"

std::string js_vips::hello() {
    return "Hello World";
}

int js_vips::add(int a, int b) {
    return a + b;
}

Napi::String js_vips::HelloWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    const std::string result = js_vips::hello();
    Napi::String returnValue = Napi::String::New(env, result);

    return returnValue;
}

Napi::Number js_vips::AddWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

    Napi::Number first = info[0].As<Napi::Number>();
    Napi::Number second = info[1].As<Napi::Number>();

    int result = js_vips::add(first.Int32Value(), second.Int32Value());

    return Napi::Number::New(env, result);
}

Napi::Object js_vips::Init(Napi::Env env, Napi::Object exports) {
    exports.Set(
        "hello", Napi::Function::New(env, js_vips::HelloWrapped)
    );

    exports.Set("add", Napi::Function::New(env, js_vips::AddWrapped));

    return exports;
}