#include <iostream>
#include "native_image.h"

using namespace vips;

NativeImage::NativeImage(const Napi::CallbackInfo& info): Napi::ObjectWrap<NativeImage>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing arguments").ThrowAsJavaScriptException();
    }

    if (info[0].IsString()) {
        std::string path = info[0].As<Napi::String>().Utf8Value();
        this->image_ = VImage::new_from_file(path.c_str(), VImage::option ()->set ("access", VIPS_ACCESS_SEQUENTIAL));
    } else if (info[0].IsObject()) {
        std::cout << "NativeImage object" << std::endl;
        Napi::Object options = info[0].As<Napi::Object>();
        CreationOptions opts = ParseCreateOptions(options);
        this->image_ = _createImage(opts);
    } else {
        Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    }
}

NativeImage::~NativeImage() {

}

Napi::Object NativeImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NativeImage", {
        InstanceMethod<&NativeImage::Countdown>("countdown", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        StaticMethod<&NativeImage::CreateSRGBImage>("createSRGBImage", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });


    // Create a persistent reference to the class constructor. This will allow
    // a function called on a class prototype and a function called on a class
    // instance to be distinguished from each other.
    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    constructor->SuppressDestruct();

    exports.Set("NativeImage", func);

    // Store the constructor as the add-on instance data. This will allow this
    // add-on to support multiple instances of itself running on multiple worker
    // threads, as well as multiple instances of itself running in different
    // contexts on the same thread.

    // By default, the value set on the environment here will be destroyed when
    // the add-on is unloaded using the `delete` operator, but it is also
    // possible to supply a custom deleter
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    return exports;
}

//
// Create an empty image with background color
// JS: NativeImage.createImage(width, height, bgColor?)
// bgColor: HTML default to #FFFFFF
//
Napi::Value NativeImage::CreateSRGBImage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing CreationOptions").ThrowAsJavaScriptException();
    }

    // Retrieve the instance data we stored during `Init()`. We only stored the
    // constructor there, so we retrieve it here to create a new instance of the
    // JS class the constructor represents.
    Napi::FunctionReference* constructor = env.GetInstanceData<Napi::FunctionReference>();
    Napi::Object obj = constructor->New({info[0]});
    return scope.Escape(napi_value(obj)).ToObject();
}

/**
 * Generate a countdown gif image
 */
Napi::Value NativeImage::Countdown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    return Napi::String::New(env, "Hello NativeImage");
}

// create an empty image
vips::VImage NativeImage::_createImage(const CreationOptions options) {
    vips::VImage image = vips::VImage::black(options.width, options.height);
    return image;
}

CreationOptions NativeImage::ParseCreateOptions(const Napi::Object& options) {
    CreationOptions opts;

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
