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

    static Napi::Value CreateImage(const Napi::CallbackInfo& info);

    // Constructor
    NativeImage(const Napi::CallbackInfo& info);

    private:
    // wrapped functions
    Napi::Value Countdown(const Napi::CallbackInfo& info);

    // create an empty image
    static vips::VImage* _createImage(int width, int height, std::string bgColor);

    void _setImage(const vips::VImage *image);

    // Internal instance of an image object
    vips::VImage *image_;

};

#endif
