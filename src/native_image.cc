#include "native_image.h"

Napi::Object NativeImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NativeImage", {
        InstanceMethod<&NativeImage::Countdown>("countdown", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        StaticMethod<&NativeImage::CreateImage>("createImage", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
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
// bgColor: default to #FFFFFF
//
Napi::Value NativeImage::CreateImage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    // Retrieve the instance data we stored during `Init()`. We only stored the
    // constructor there, so we retrieve it here to create a new instance of the
    // JS class the constructor represents.
    Napi::FunctionReference* constructor = env.GetInstanceData<Napi::FunctionReference>();

    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Arguments width and height are required").ThrowAsJavaScriptException();
    }

    int width = info[0].As<Napi::Number>().Int32Value();
    int height = info[1].As<Napi::Number>().Int32Value();

    if (info.Length() >= 3 && !info[2].IsString()) {
        Napi::TypeError::New(env, "Argument bgColor must be a string").ThrowAsJavaScriptException();
    }

    std::string bgColor = info.Length() >= 3 ? info[2].As<Napi::String>().Utf8Value() : "#FFFFFF";
    const vips::VImage *image = _createImage(width, height, bgColor);

    Napi::Object obj = constructor->New({});
    NativeImage* nativeImage = Napi::ObjectWrap<NativeImage>::Unwrap(obj);
    nativeImage->_setImage(image);

    return Napi::ObjectWrap<NativeImage>::New(obj);
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

// create an empty image
vips::VImage* NativeImage::_createImage(int width, int height, std::string bgColor) {
    vips::VImage *image = vips::VImage.black(width, height);
    return image;
}

void NativeImage::_setImage(const vips::VImage *image) {
    this->image_ = image;
}

