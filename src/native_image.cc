#include "native_image.h"

using namespace vips;
using namespace ni;

NativeImage::NativeImage(const Napi::CallbackInfo& info): Napi::ObjectWrap<NativeImage>(info) {
    mode_ = ImageMode::IMAGE;
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing arguments").ThrowAsJavaScriptException();
    }

    this->mode_ = ImageMode::IMAGE;
    // First argument is the path to the image file or configuration file
    if (info[0].IsString()) {
        std::string path = info[0].As<Napi::String>().Utf8Value();
        this->image_ = VImage::new_from_file(path.c_str(), VImage::option ()->set ("access", VIPS_ACCESS_SEQUENTIAL));
        this->imageOriginalPath_ = path;
    } else if (info[0].IsObject()) {
        if (info.Length() < 2 || !info[1].IsNumber()) {
            Napi::TypeError::New(env, "Missing mode").ThrowAsJavaScriptException();
        }
        int mode = info[1].As<Napi::Number>().Int32Value();
        auto options = info[0].As<Napi::Object>();

        if (mode == static_cast<int>(ImageMode::TEXTIMAGE)) {
            this->mode_ = ImageMode::TEXTIMAGE;
            try {
                NativeTextImageOptions textOptions = toNativeTextImageOptions(options);
                this->image_ = newImageWithTexts(textOptions);
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
            }
        } else {
            Napi::TypeError::New(env, "Invalid mode").ThrowAsJavaScriptException();
        }
    } else {
        Napi::TypeError::New(env, "Invalid the first argument.").ThrowAsJavaScriptException();
    }
}

Napi::Object NativeImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NativeImage", {
        StaticMethod<&NativeImage::NewTextImage>("newTextImage", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::RebuildTextElementCache>("rebuildTextElementCache", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::RebuildTextElementCache2>("rebuildTextElementCache2", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::AddTextElements>("addTextElements", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::Animation>("animation", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::Save>("save", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::Encode>("encode", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
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
// Create an Text image
//
Napi::Value NativeImage::NewTextImage(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing TextImageOptions").ThrowAsJavaScriptException();
    }

    Napi::Value mode = Napi::Number::New(env, static_cast<int>(ImageMode::TEXTIMAGE));

    // Retrieve the instance data we stored during `Init()`. We only stored the
    // constructor there, so we retrieve it here to create a new instance of the
    // JS class the constructor represents.
    Napi::FunctionReference* constructor = env.GetInstanceData<Napi::FunctionReference>();
    Napi::Object obj = constructor->New({info[0], mode});
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value NativeImage::RebuildTextElementCache(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing TextElement to be cached").ThrowAsJavaScriptException();
    }

    if (!info[0].IsArray()) {
        Napi::TypeError::New(env, "Invalid TextElement array").ThrowAsJavaScriptException();
    }
    Napi::Array textArray = info[0].As<Napi::Array>();

    std::vector<ni::NativeTextElement> textElements;
    for (uint32_t i = 0; i < textArray.Length(); i++) {
        if (!textArray.Get(i).IsObject()) {
            Napi::TypeError::New(env, "Invalid TextElement object").ThrowAsJavaScriptException();
        }
        Napi::Object textObj = textArray.Get(i).As<Napi::Object>();
        ni::NativeTextElement element = ni::toNativeTextElement(textObj);
        textElements.push_back(element);
    }

    this->rebuildTextImageCache(textElements);

    return Napi::Number::New(env, textElements.size());
}

Napi::Value NativeImage::RebuildTextElementCache2(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Missing TextElement to be cached").ThrowAsJavaScriptException();
    }

    if (!info[0].IsArray()) {
        Napi::TypeError::New(env, "Invalid TextElement array").ThrowAsJavaScriptException();
    }
    Napi::Array textArray = info[0].As<Napi::Array>();

    std::vector<ni::NativeTextElement> textElements;
    for (uint32_t i = 0; i < textArray.Length(); i++) {
        if (!textArray.Get(i).IsObject()) {
            Napi::TypeError::New(env, "Invalid TextElement object").ThrowAsJavaScriptException();
        }
        Napi::Object textObj = textArray.Get(i).As<Napi::Object>();
        NativeTextElement element = ni::toNativeTextElement(textObj);
        textElements.push_back(element);
    }

    int trimLeft = info[1].As<Napi::Number>().Int32Value();

    this->rebuildTextImageCache2(textElements, trimLeft);

    return Napi::Number::New(env, textElements.size());
}

Napi::Value NativeImage::AddTextElements(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing TextElements to be added").ThrowAsJavaScriptException();
    }

    if (!info[0].IsArray()) {
        Napi::TypeError::New(env, "Invalid TextElement array").ThrowAsJavaScriptException();
    }
    Napi::Array textArray = info[0].As<Napi::Array>();

    std::vector<NativeTextElement> textElements;
    for (uint32_t i = 0; i < textArray.Length(); i++) {
        if (!textArray.Get(i).IsObject()) {
            Napi::TypeError::New(env, "Invalid TextElement object").ThrowAsJavaScriptException();
        }
        Napi::Object textObj = textArray.Get(i).As<Napi::Object>();
        NativeTextElement element = toNativeTextElement(textObj);
        textElements.push_back(element);
    }

    this->image_ = this->addTextElements(this->image_, textElements, this->imageElements_);

    return Napi::Number::New(env, textElements.size());
}

Napi::Value NativeImage::Animation(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    std::string outputFilePath;

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing frames").ThrowAsJavaScriptException();
    }

    if (!info[0].IsArray()) {
        Napi::TypeError::New(env, "Invalid frames, not an array").ThrowAsJavaScriptException();
    }
    Napi::Array framesArray = info[0].As<Napi::Array>();

    if (info.Length() >= 2) {
        // The second parameter is the output filepath
        if (!info[1].IsString()) {
            Napi::TypeError::New(env, "Invalid output file path").ThrowAsJavaScriptException();
        }
        outputFilePath = info[1].As<Napi::String>().Utf8Value();
    }

    // Iterate
    std::vector<std::vector<NativeTextElement>> frames;
    frames.reserve(framesArray.Length());
    for (uint32_t i = 0; i < framesArray.Length(); i++) {
        if (!framesArray.Get(i).IsArray()) {
            Napi::TypeError::New(env, "Invalid frame, not an array").ThrowAsJavaScriptException();
        }
        Napi::Array frameArray = framesArray.Get(i).As<Napi::Array>();

        std::vector<NativeTextElement> textElements;
        textElements.reserve(frameArray.Length());
        for (uint32_t j = 0; j < frameArray.Length(); j++) {
            if (!frameArray.Get(j).IsObject()) {
                Napi::TypeError::New(env, "Invalid TextElement object").ThrowAsJavaScriptException();
            }
            Napi::Object textObj = frameArray.Get(j).As<Napi::Object>();
            NativeTextElement element = toNativeTextElement(textObj);
            textElements.push_back(element);
        }

        frames.push_back(textElements);
    }

    // Render the animation
    VImage gifImage = illustrationAnimation(frames);

    if (outputFilePath.empty()) {
        // write to a formatted memory buffer
        void *buf;
        size_t size = encode(gifImage, "gif", buf);
        const auto jsBuf = Napi::Buffer<char>::Copy(env, static_cast<char*>(buf), size);
        g_free(buf);
        return jsBuf;
    } else {
        gifImage.write_to_file(outputFilePath.c_str());
        return Napi::String::New(env, outputFilePath);
    }

}

Napi::Value NativeImage::Encode(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing extension").ThrowAsJavaScriptException();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Invalid extension").ThrowAsJavaScriptException();
    }

    std::string extension = info[0].As<Napi::String>().Utf8Value();

    void *buf;
    size_t size = encode(this->image_, extension, buf);
    const auto jsBuf = Napi::Buffer<char>::Copy(env, static_cast<char*>(buf), size);
    g_free(buf);

    return jsBuf;
}

/**
 * Save the image to a file
 */
Napi::Value NativeImage::Save(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() == 0) {
        Napi::TypeError::New(env, "Missing file path").ThrowAsJavaScriptException();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Invalid file path").ThrowAsJavaScriptException();
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();
    save(this->image_, path);

    return Napi::Number::New(env, 0);
}

VImage NativeImage::newImageWithBgColor(int width, int height, const std::vector<double>& bgColor) {

    VImage emptyImage = VImage::black(width, height, VImage::option()->set("bands", 3)) + bgColor;
    return emptyImage.cast(VipsBandFormat::VIPS_FORMAT_UCHAR).copy(VImage::option()->set("interpretation", VIPS_INTERPRETATION_sRGB));
}

VImage NativeImage::newImageOfTextElement(const std::string &text, const std::string &fontFile, const std::vector<double> &color, const std::vector<double> &bgColor) {
    auto genOpts = VImage::option();
    if (!fontFile.empty()) {
        genOpts->set("fontfile", fontFile.c_str());
    }

    return VImage::text(text.c_str(), genOpts).ifthenelse(color, bgColor, VImage::option()->set("blend", true));
}

VImage NativeImage::newImageWithTexts(const NativeTextImageOptions &options) {

    // Draw background image
    VImage canvas = newImageWithBgColor(options.width, options.height, options.bgColor);

    if (options.texts.empty()) {
        return canvas;
    }

    std::vector<VImage> elements;
    return addTextElements(canvas, options.texts, elements);
}

VImage NativeImage::addTextElements(const VImage &canvas, const std::vector<NativeTextElement> &elements, const std::vector<VImage> &imageElements) {
    std::vector<VImage> textImages;
    textImages.reserve(elements.size() + 1);
    std::vector<int> xPos;
    xPos.reserve(elements.size());
    std::vector<int> yPos;
    xPos.reserve(elements.size());
    std::vector<int> modes = {VipsBlendMode::VIPS_BLEND_MODE_OVER};

    textImages.push_back(canvas);
    for (const auto& element : elements) {
        if (element.cacheIndex >= 0 && element.cacheIndex < static_cast<int>(imageElements.size())) {
            // Hit the cache
            const int y = ( element.containerHeight - imageElements.at(element.cacheIndex).height() ) / 2;
            const int x = ( element.containerWidth - imageElements.at(element.cacheIndex).width() ) / 2;
            textImages.push_back(imageElements.at(element.cacheIndex));
            xPos.push_back(element.offsetLeft + x);
            yPos.push_back(element.offsetTop + y);
            continue;
        } else {
            VImage textImage = newImageOfTextElement(element.text, element.fontFile, element.color, element.bgColor);
            textImages.push_back(textImage);
            const int y = ( element.containerHeight - textImage.height() ) / 2;
            const int x = ( element.containerWidth - textImage.width() ) / 2;
            xPos.push_back(element.offsetLeft + x);
            yPos.push_back(element.offsetTop + y);
        }
    }

    return VImage::composite(textImages, modes, VImage::option()->set("x", xPos)->set("y", yPos));

}

void NativeImage::rebuildTextImageCache(const std::vector<NativeTextElement> &texts) {
    this->imageElements_.clear();

    std::vector<std::pair<int, int>> sizes;
    sizes.reserve(texts.size());
    for (const auto& text : texts) {
        VImage textImage = newImageOfTextElement(text.text, text.fontFile, text.color, text.bgColor);
        this->imageElements_.push_back(textImage);

        // Compare the height
        const std::pair<int, int> s = {textImage.width(), textImage.height()};
        // find the same height in the vector
        const auto result = std::find_if(sizes.begin(), sizes.end(), [&s](const std::pair<int, int>& p) {
            return p.second == s.second;
        });

        if (result == sizes.end()) {
            // Not found
            if (!sizes.empty()) {
                std::clog << "Different text image sizes are found on [" << text.text << "], size" << textImage.width() << "x" << textImage.height() << ". It will impact alignment when compose an image." << std::endl;
            } else {
                std::clog << "Cached Text image size: " << textImage.width() << "x" << textImage.height() << std::endl;
            }
            sizes.push_back(s);
        }
    }
}

void NativeImage::rebuildTextImageCache2(const std::vector<NativeTextElement> &texts, int trimLeft) {
    this->imageElements_.clear();
    this->imageElements_.reserve(texts.size());

    std::vector<std::pair<int, int>> sizes;
    sizes.reserve(texts.size());
    for (const auto& text : texts) {
        VImage textImage = newImageOfTextElement(text.text, text.fontFile, text.color, text.bgColor);

        // Trim the left side of the image
        if (trimLeft > 0) {
            textImage = textImage.extract_area(trimLeft, 0, textImage.width() - trimLeft, textImage.height());
        }

        this->imageElements_.push_back(textImage);

        // Compare the height
        const std::pair<int, int> s = {textImage.width(), textImage.height()};

        // find the same height in the vector
        const auto result = std::find_if(sizes.begin(), sizes.end(), [&s](const std::pair<int, int>& p) {
            return p.second == s.second;
        });

        if (result == sizes.end()) {
            // Not found
            if (!sizes.empty()) {
                std::clog << "Different text image sizes are found on [" << text.text << "], size" << textImage.width() << "x" << textImage.height() << std::endl;
            } else {
                std::clog << "Cached Text image size: " << textImage.width() << "x" << textImage.height() << std::endl;
            }
            sizes.push_back(s);
        }
    }
}

VImage NativeImage::illustrationAnimation(const std::vector<std::vector<NativeTextElement>> &frames) {
    std::vector<VImage> pages;
    pages.reserve(frames.size());

    for (const auto& frame : frames) {
        VImage page = addTextElements(this->image_, frame, this->imageElements_);
        pages.push_back(page);
    }

    // Join a set of pages vertically to make a multipage image
    VImage animation = VImage::arrayjoin(pages, VImage::option()->set("across", 1));
    VImage gifData = animation.copy();
    gifData.set("page-height", this->image_.height());

    // frame delays are in milliseconds ... 300 is pretty slow!
    std::vector<int> delayArray(pages.size(), 1000);
    gifData.set("delay", delayArray);

    return gifData;
}

std::string NativeImage::save(const vips::VImage &image, const std::string &path) {
    image.write_to_file(path.c_str());
    return path;
}

size_t NativeImage::encode(const vips::VImage &image, const std::string &extension, void *&encodedBuffer) {
    // write to a formatted memory buffer
    size_t size;

    std::string format = "." + extension;
    image.write_to_buffer(format.c_str(), &encodedBuffer, &size);
    return size;
}
