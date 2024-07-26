#include "actual_class.h"

ActualClass::ActualClass(double value){
    this->value_ = value;
}

double ActualClass::getValue() {
    return this->value_;
}

double ActualClass::add(double toAdd) {
    this->value_ += toAdd;
    return this->value_;
}


Napi::FunctionReference ActualClassWrapped::constructor;

Napi::Object ActualClassWrapped::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "ActualClassWrapped", {
        InstanceMethod("add", &ActualClassWrapped::Add),
        InstanceMethod("getValue", &ActualClassWrapped::GetValue),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("ActualClass", func);
    return exports;
}

ActualClassWrapped::ActualClassWrapped(const Napi::CallbackInfo& info) : Napi::ObjectWrap<ActualClassWrapped>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();
    if (length != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }
    
    Napi::Number value = info[0].As<Napi::Number>();
    this->actualClass_ = new ActualClass(value.DoubleValue());
}

Napi::Value ActualClassWrapped::GetValue(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    double num = this->actualClass_->getValue();
    return Napi::Number::New(env, num);
}

Napi::Value ActualClassWrapped::Add(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if ( info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

    Napi::Number toAdd = info[0].As<Napi::Number>();
    double answer = this->actualClass_->add(toAdd.DoubleValue());

    return Napi::Number::New(env, answer);
}