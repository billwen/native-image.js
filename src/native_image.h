#ifndef NATIVE_IMAGE_H
#define NATIVE_IMAGE_H

#include <map>
#include <napi.h>
#include <vips/vips8>

enum class ImageMode {
    IMAGE,
    COUNTDOWN
};

enum class CountdownMomentPart {
  DAYS,
  HOURS,
  MINUTES,
  SECONDS
};

const int lengthOfCountdownMomentParts = static_cast<int>(CountdownMomentPart::SECONDS) + 1;
const int totalOfDigits = 100;

const std::string countdownMomentPartNames[lengthOfCountdownMomentParts] = {
  "days",
  "hours",
  "minutes",
  "seconds"
};

struct CreationOptions {
    int width {0};
    int height {0};
    std::string bgColor {"#ffffff"};
};

struct ColoredTextOptions {
    std::vector<u_char> textColor {255, 255, 255};
    std::string font;
    std::string fontFile;
    int width {0};
    int height {0};
    VipsCompassDirection textAlignment {VipsCompassDirection::VIPS_COMPASS_DIRECTION_CENTRE};
    int paddingTop {0};
    int paddingBottom {0};
};

template <typename T>
struct Dimension2D {
    T width;
    T height;
};

template <typename T>
struct CountdownMoment {
    T days;
    T hours;
    T minutes;
    T seconds;
};

struct Position2D : Dimension2D<int> {
    int x {0};
    int y {0};
};

struct CountdownComponentStyle {
    std::string color {"#ffffff"};
    std::string font;
    std::string fontFile;
    int width {0};
    int height {0};
    VipsCompassDirection textAlignment {VipsCompassDirection::VIPS_COMPASS_DIRECTION_CENTRE};
};

struct CountdownComponentPosition {
  // Position of the component - required
  Position2D            position;
};

struct CountdownComponent : CountdownComponentPosition, CountdownComponentStyle {
    // Text to display - required
    std::string text;
    int paddingTop {0};
    int paddingBottom {0};
};

struct CountdownDigits {
    CountdownComponentPosition positions[lengthOfCountdownMomentParts];
    CountdownComponentStyle style;
    std::string textTemplate;
};

struct CountdownOptions : CreationOptions {
    // labels
    std::map<std::string, CountdownComponent> labels {};

    // digits
    CountdownDigits digits;
};

class NativeImage: public Napi::ObjectWrap<NativeImage> {
  public:
    // Constructor
    explicit NativeImage(const Napi::CallbackInfo& info);

    void initCountdownAnimation();

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    // Create an empty image with background color
    static Napi::Value CreateSRGBImage(const Napi::CallbackInfo& info);

    static Napi::Value CreateCountdownAnimation(const Napi::CallbackInfo& info);
    Napi::Value RenderCountdownAnimation(const Napi::CallbackInfo& info);

    // Create an text image with color
    static Napi::Value Text(const Napi::CallbackInfo& info);
    
    // Draw text on the image
    Napi::Value DrawText(const Napi::CallbackInfo& info);

    // Save the image to a file
    Napi::Value Save(const Napi::CallbackInfo& info);

    // create an empty image
    static vips::VImage createRGBImage(const CreationOptions& options);

    static vips::VImage coloredTextImage(const std::string &text, const ColoredTextOptions& options);

    vips::VImage renderCountdownAnimation(const std::vector<int>& duration, int frames);

    //
    // Help functions
    //
    static CreationOptions            parseCreationOptions(const Napi::Object& options);
    static CountdownOptions           parseCountdownOptions(const Napi::Object& options);
    static CountdownComponent         parseCountdownComponent(const Napi::Object& options, bool ignoreText = false);
    static Position2D                 parsePosition2D(const Napi::Object& options);
    static CountdownComponentPosition parseCountdownComponentPosition(const Napi::Object& options);
    static CountdownComponentStyle    parseCountdownComponentStyle(const Napi::Object& options);
    static std::vector<int>           parseCountdownMomentWithNumber(const Napi::Object& options);

    static std::vector<u_char>        hexadecimalColorToARGB(const std::string& hex);
    static std::vector<int>           minusOneSecondToDuration(const std::vector<int>& duration);

    //
    // Internal instance of an image object
    //
    vips::VImage image_;
    ImageMode mode_;

    // Countdown animation
    CountdownOptions countdownOptions_;
    std::vector<vips::VImage> countdownDigits_;
};

#endif
