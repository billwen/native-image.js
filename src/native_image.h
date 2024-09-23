#ifndef NATIVE_IMAGE_H
#define NATIVE_IMAGE_H

#include <vector>
#include <iostream>
#include <map>
#include <napi.h>
#include <vips/vips8>

namespace ni {
    enum class ImageMode {
        IMAGE,
        TEXTIMAGE,
        NUMBER_OF_MODES
    };

    struct NativeTextElement {
        std::string text;
        std::string fontFile;
        std::vector<double> color {0.0, 0.0, 0.0};
        std::vector<double> bgColor {255.0, 255.0, 255.0};
        int containerWidth {0};
        int containerHeight {0};
        int offsetTop {0};
        int offsetLeft {0};
        int cacheIndex {-1};
    };

    struct NativeTextImageOptions {
        int width {0};
        int height {0};
        std::vector<double> bgColor {255.0, 255.0, 255.0};

        // Texts on template
        std::vector<NativeTextElement> texts;
    };

    // Convert Napi::Object to native types
    NativeTextElement        toNativeTextElement(const Napi::Object& options);
    NativeTextImageOptions   toNativeTextImageOptions(const Napi::Object& options);
}

class NativeImage: public Napi::ObjectWrap<NativeImage> {
  public:
    // Constructor
    explicit NativeImage(const Napi::CallbackInfo& info);

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);


    // Create an empty image with background color
    static vips::VImage newImageWithBgColor(int width, int height, const std::vector<double>& bgColor);

    // Create a text image with color and background color
    static vips::VImage newImageOfTextElement(const std::string &text, const std::string &fontFile, const std::vector<double> &color, const std::vector<double> &bgColor);

    // Create an text image with color
    static vips::VImage newImageWithTexts(const ni::NativeTextImageOptions& options);

    static vips::VImage addTextElements(const vips::VImage &canvas, const std::vector<ni::NativeTextElement> &elements, const std::vector<vips::VImage> &imageElements);

    // rebuild the cache of images for performance
    void rebuildTextImageCache(const std::vector<ni::NativeTextElement> &texts);
    void rebuildTextImageCache2(const std::vector<ni::NativeTextElement> &texts, int trimLeft);

    vips::VImage illustrationAnimation(const std::vector<std::vector<ni::NativeTextElement>> &frames);

    static std::string save(const vips::VImage &image, const std::string &path);

    static size_t encode(const vips::VImage &image, const std::string &extension, void *&encodedBuffer);

private:
    // Create an colored text image
    static Napi::Value  NewTextImage(const Napi::CallbackInfo& info);

    // Build text images cache
    Napi::Value         RebuildTextElementCache(const Napi::CallbackInfo& info);
    Napi::Value         RebuildTextElementCache2(const Napi::CallbackInfo& info);

    // Add text elements to the image
    Napi::Value         AddTextElements(const Napi::CallbackInfo& info);

    // Generate an illustration animation
    Napi::Value         Animation(const Napi::CallbackInfo& info);

    // Encode the image to a specific format
    Napi::Value         Encode(const Napi::CallbackInfo& info);

    // Save the image to a file
    Napi::Value         Save(const Napi::CallbackInfo& info);

    //
    // Internal instance of an image object
    //

    // Internal image object
    vips::VImage                image_;

    // Internal image elements cache
    std::vector<vips::VImage>   imageElements_;

    // Image mode, either IMAGE or COUNTDOWN
    ni::ImageMode               mode_;

    // original file path
    std::string                 imageOriginalPath_;
};

#endif
