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
        InstanceMethod<&NativeImage::DrawText>("drawText", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&NativeImage::Save>("save", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
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
 * Draw text on the image
 */
Napi::Value NativeImage::DrawText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Missing parameters").ThrowAsJavaScriptException();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Invalid text").ThrowAsJavaScriptException();
    }
    const std::string text = info[0].As<Napi::String>().Utf8Value();

    if (text.length() == 0) {
        Napi::TypeError::New(env, "Invalid text").ThrowAsJavaScriptException();
    }

    if (!info[1].IsNumber()) {
        Napi::TypeError::New(env, "Invalid topX").ThrowAsJavaScriptException();
    }
    const double topX = info[1].As<Napi::Number>().DoubleValue();

    if (!info[2].IsNumber()) {
        Napi::TypeError::New(env, "Invalid topY").ThrowAsJavaScriptException();
    }
    const double topY = info[2].As<Napi::Number>().DoubleValue();

    std::string color = "#000000";

    auto opts = VImage::option();

    if (info.Length() > 3) {
        if (!info[3].IsObject()) {
            Napi::TypeError::New(env, "Invalid options").ThrowAsJavaScriptException();
        }

        Napi::Object options = info[3].As<Napi::Object>();

        if (options.Has("font")) {
            if (!options.Get("font").IsString()) {
                Napi::TypeError::New(env, "Invalid font").ThrowAsJavaScriptException();
            }
            opts->set("font", options.Get("font").As<Napi::String>().Utf8Value().c_str());
        }

        if (options.Has("fontFile")) {
            if (!options.Get("fontFile").IsString()) {
                Napi::TypeError::New(env, "Invalid fontFile").ThrowAsJavaScriptException();
            }
            opts->set("fontfile", options.Get("fontFile").As<Napi::String>().Utf8Value().c_str());
        }

        if (options.Has("color")) {
            if (!options.Get("color").IsString()) {
                Napi::TypeError::New(env, "Invalid color").ThrowAsJavaScriptException();
            }
            color = options.Get("color").As<Napi::String>().Utf8Value();

            if (color.at(0) != '#') {
                Napi::TypeError::New(env, "Invalid color").ThrowAsJavaScriptException();
            }           
        }
    }

    auto textColor = htmlHexStringToARGB(color);

    // Create a new text image

    // this renders the text to a one-band image ... set width to the pixels across
    // of the area we want to render to to have it break lines for you
    VImage textImage = VImage::text(text.c_str(), opts);
    
    // make a constant image the size of $text, but with every pixel red ... tag it
    // as srgb
    const std::vector<double> bgText = {double(textColor[1]), double(textColor[2]), double(textColor[3])};
    VImage textImageBg = textImage.new_from_image(bgText).copy(VImage::option()->set("interpretation", VIPS_INTERPRETATION_sRGB));

    // use the text mask as the alpha for the constant red image
    VImage overlay = textImageBg.bandjoin(textImage);

    // composite the text on the image
    this->image_ = this->image_.composite(overlay, VIPS_BLEND_MODE_OVER, VImage::option()->set("x", topX)->set("y", topY));

    return Napi::Number::New(env, 0);
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
    this->image_.write_to_file(path.c_str());

    return Napi::Number::New(env, 0);
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
    std::vector<u_char> bgColor = htmlHexStringToARGB(options.bgColor);

    // Make a 1x1 pixel with the red channel and cast it to provided format.
    vips::VImage redChannel = vips::VImage::new_from_memory(&bgColor[1], 1, 1, 1,1, VipsBandFormat::VIPS_FORMAT_UCHAR);
    vips::VImage pixel = vips::VImage::black(1, 1).add(redChannel).cast(VipsBandFormat::VIPS_FORMAT_UCHAR);

    // Extend this 1x1 pixel to match the origin image dimensions
    vips::VImage image = pixel.embed(0, 0, options.width, options.height, VImage::option()->set("extend", VIPS_EXTEND_COPY));

    // Ensure that the interpretation of the image is set.
    vips::VImage imageInterpreted = image.copy(VImage::option()->set("interpretation", VIPS_INTERPRETATION_sRGB));

    // Bandwise join the rest of the channels including the alpha channel.
    const double dg = bgColor[2];
    const double db = bgColor[3];
    const double da = bgColor[0];
    vips::VImage imageJoined = imageInterpreted.bandjoin({dg, db, da});

    return imageJoined;
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

/**
 * Convert hex color string to RGB
 * 
 * Allowed formats:
 * [#]RGB
 * [#]ARGB
 * [#]RRGGBB
 * [#]AARRGGBB
 *
 * @param hex color hex string, should be start with "#" and the rest length is 3, 4, 6 or 8 characters
 * @return array a decimal ARGB array, return black if the hex string is invalid
 * 
 */
std::vector<u_char> NativeImage::htmlHexStringToARGB(const std::string& hex) {
    const std::vector<u_char> defaultColor = {0, 0, 0, 0};
    if (hex.length() == 0) {
        return defaultColor;
    }

    if (hex.length() != 4 && hex.length() != 5 && hex.length() != 7 && hex.length() != 9) {
        return defaultColor;
    }

    if (hex.at(0) != '#') {
        return defaultColor;
    }

    std::string a,r,g,b;
    int withAlpha = 0;

    if (hex.length() == 4 || hex.length() == 5) {
        if ( hex.length() == 5 ) {
            withAlpha = 1;
            a = hex.substr(1, 1);
        } else {
            a = "FF";
        }

        r = hex.substr(1 + withAlpha, 1) + hex.substr(1 + withAlpha, 1);
        g = hex.substr(2 + withAlpha, 1) + hex.substr(2 + withAlpha, 1);
        b = hex.substr(3 + withAlpha, 1) + hex.substr(3 + withAlpha, 1);
    } else if (hex.length() == 7 || hex.length() == 9) {
        if ( hex.length() == 9 ) {
            withAlpha = 1;
            a = hex.substr(1, 2);
        } else {
            a = "FF";
        }
        r = hex.substr(1 + 2*withAlpha, 2);
        g = hex.substr(3 + 2*withAlpha, 2);
        b = hex.substr(5 + 2*withAlpha, 2);
    }

    // convert hex to decimal
    u_char alpha = std::stoi(a, nullptr, 16);
    u_char red = std::stoi(r, nullptr, 16);
    u_char green = std::stoi(g, nullptr, 16);
    u_char blue = std::stoi(b, nullptr, 16);

    return {alpha, red, green, blue};
}
