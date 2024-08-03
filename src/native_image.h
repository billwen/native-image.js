#ifndef NATIVE_IMAGE_H
#define NATIVE_IMAGE_H

#include <napi.h>
#include <vips/vips8>

typedef struct {
    int width;
    int height;
    std::string bgColor;
} CreationOptions;

typedef struct {
    // Image width
    int width;

    // Image height
    int height;

    // Background color, e.g. "#FF0000"
    std::string bgColor;

    // output file path
    std::string outFilePath;
} CountdownOptions;

class NativeImage: public Napi::ObjectWrap<NativeImage> {
  public:
    // Constructor
    NativeImage(const Napi::CallbackInfo& info);
    ~NativeImage();

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    // Create an empty image with background color
    static Napi::Value CreateSRGBImage(const Napi::CallbackInfo& info);

    Napi::Value DrawText(const Napi::CallbackInfo& info);

    // Save the image to a file
    Napi::Value Save(const Napi::CallbackInfo& info);

    // wrapped functions
    Napi::Value Countdown(const Napi::CallbackInfo& info);

    // create an empty image
    static vips::VImage _createImage(const CreationOptions options);

    //
    // Help functions
    //
    static CreationOptions ParseCreateOptions(const Napi::Object& options);
    static std::vector<u_char> htmlHexStringToARGB(const std::string& hex);

    //
    // Internal instance of an image object
    //
    vips::VImage image_;

};

#endif
