#ifndef NATIVE_IMAGE_H
#define NATIVE_IMAGE_H

#include <napi.h>
#include <vips/vips8>

class NativeImage: public Napi::ObjectWrap<NativeImage> {

    struct CountdownOptions {
        // Image width
        int width;

        // Image height
        int height;

        // Background color, e.g. "#FF0000"
        std::string bgColor;

        // output file path
        std::string outFilePath;
    };

    public:

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    static Napi::Object CreateEmpty(Napi::Env env, Napi::Value arg);

    // Constructor
    NativeImage(const Napi::CallbackInfo& info);

    private:
    // reference to store the class definition that needs to be exported to JS
    static Napi::FunctionReference constructor;

    // wrapped functions
    Napi::Value Countdown(const Napi::CallbackInfo& info);

    // Internal instance of an image object
    vips::VImage *image_;

};

#endif
