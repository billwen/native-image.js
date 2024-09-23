#include "native_image.h"

using namespace ni;

NativeTextElement ni::toNativeTextElement(const Napi::Object& options) {
    NativeTextElement element;

    if (!options.Has("text") || !options.Get("text").IsString()) {
        throw std::invalid_argument("Missing text attribute");
    }
    element.text = options.Get("text").As<Napi::String>().Utf8Value();

    if (options.Has("fontFile") && options.Get("fontFile").IsString()) {
        element.fontFile = options.Get("fontFile").As<Napi::String>().Utf8Value();
    }

    if (!options.Has("color") || !options.Get("color").IsArray()) {
        throw std::invalid_argument("Missing color attribute");
    }
    auto colorArray = options.Get("color").As<Napi::Array>();
    if (colorArray.Length() != 3) {
        throw std::invalid_argument("color must be an array of 3 elements");
    }
    for (uint32_t i = 0; i < colorArray.Length(); i++) {
        if (!colorArray.Get(i).IsNumber()) {
            throw std::invalid_argument("color must be an array of 3 numbers");
        }
        element.color[i] = colorArray.Get(i).As<Napi::Number>().DoubleValue();
    }

    if (!options.Has("bgColor") || !options.Get("bgColor").IsArray()) {
        throw std::invalid_argument("Missing bgColor attribute");
    }
    auto bgColorArray = options.Get("bgColor").As<Napi::Array>();
    if (bgColorArray.Length() != 3) {
        throw std::invalid_argument("bgColor must be an array of 3 elements");
    }
    for (uint32_t i = 0; i < bgColorArray.Length(); i++) {
        if (!bgColorArray.Get(i).IsNumber()) {
            throw std::invalid_argument("bgColor must be an array of 3 numbers");
        }
        element.bgColor[i] = bgColorArray.Get(i).As<Napi::Number>().DoubleValue();
    }

    if (!options.Has("containerWidth") || !options.Get("containerWidth").IsNumber()) {
        throw std::invalid_argument("Missing containerWidth attribute");
    }
    element.containerWidth = options.Get("containerWidth").As<Napi::Number>().Int32Value();

    if (!options.Has("containerHeight") || !options.Get("containerHeight").IsNumber()) {
        throw std::invalid_argument("Missing containerHeight attribute");
    }
    element.containerHeight = options.Get("containerHeight").As<Napi::Number>().Int32Value();

    if (options.Has("offsetTop") && options.Get("offsetTop").IsNumber()) {
        element.offsetTop = options.Get("offsetTop").As<Napi::Number>().Int32Value();
    }

    if (options.Has("offsetLeft") && options.Get("offsetLeft").IsNumber()) {
        element.offsetLeft = options.Get("offsetLeft").As<Napi::Number>().Int32Value();
    }

    if (options.Has("cacheIndex") && options.Get("cacheIndex").IsNumber()) {
        element.cacheIndex = options.Get("cacheIndex").As<Napi::Number>().Int32Value();
    }

    return element;
}

NativeTextImageOptions ni::toNativeTextImageOptions(const Napi::Object& options) {
    NativeTextImageOptions textOptions;

    if (!options.Has("width") || !options.Get("width").IsNumber()) {
        throw std::invalid_argument("Missing width attribute");
    }
    textOptions.width = options.Get("width").As<Napi::Number>().Int32Value();

    if (!options.Has("height") || !options.Get("height").IsNumber()) {
        throw std::invalid_argument("Missing height attribute");
    }
    textOptions.height = options.Get("height").As<Napi::Number>().Int32Value();

    if (!options.Has("bgColor") || !options.Get("bgColor").IsArray()) {
        throw std::invalid_argument("Missing bgColor attribute");
    }
    auto bgColorArray = options.Get("bgColor").As<Napi::Array>();
    if (bgColorArray.Length() != 3) {
        throw std::invalid_argument("bgColor must be an array of 3 elements");
    }
    for (uint32_t i = 0; i < bgColorArray.Length(); i++) {
        if (!bgColorArray.Get(i).IsNumber()) {
            throw std::invalid_argument("bgColor must be an array of 3 numbers");
        }
        textOptions.bgColor[i] = bgColorArray.Get(i).As<Napi::Number>().DoubleValue();
    }

    if (!options.Has("texts") || !options.Get("texts").IsArray()) {
        throw std::invalid_argument("Missing texts attribute");
    }
    auto textArray = options.Get("texts").As<Napi::Array>();
    for (uint32_t i = 0; i < textArray.Length(); i++) {
        if (!textArray.Get(i).IsObject()) {
            throw std::invalid_argument("texts must be an array of objects");
        }
        Napi::Object textObj = textArray.Get(i).As<Napi::Object>();
        NativeTextElement element = toNativeTextElement(textObj);
        textOptions.texts.push_back(element);
    }

    return textOptions;
}
