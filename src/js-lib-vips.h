#include <napi.h>

namespace js_lib_vips {

    /**
     * Types
     */
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

    /**
     * Internal functions
     */
    CountdownOptions ParseOptions(const Napi::Object& options);
    
    /**
     * Pure C++ functions
     */
    std::string countdown();

    /**
     * Functions exposed to JS
     */
    Napi::String Countdown(const Napi::CallbackInfo& info);
    Napi::Object InitLibVips(Napi::Env env, Napi::Object exports);
}
