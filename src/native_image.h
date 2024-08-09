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

    void init_countdown_animation();

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    // Create an empty image with background color
    static Napi::Value CreateSRGBImage(const Napi::CallbackInfo& info);
    // Draw text on the image
    Napi::Value DrawText(const Napi::CallbackInfo& info);
    // Save the image to a file
    Napi::Value Save(const Napi::CallbackInfo& info);

    static Napi::Value CreateCountdownAnimation(const Napi::CallbackInfo& info);
    Napi::Value RenderCountdownAnimation(const Napi::CallbackInfo& info);

    // Create an text image with color
    static Napi::Value CreateText(const Napi::CallbackInfo& info);

    // create an empty image
    static vips::VImage create_rgb_Image(const CreationOptions& options);

    static vips::VImage colored_text_image(const std::string &text, const ColoredTextOptions& options);

    vips::VImage render_countdown_animation(const std::vector<int>& duration, int frames);

    //
    // Help functions
    //
    static Position2D                 parse_position_2d(const Napi::Object& options);
    static CreationOptions            parse_creation_options(const Napi::Object& options);

    static CountdownComponent         parse_countdown_component(const Napi::Object& options, bool ignoreText = false);
    static CountdownComponentPosition parse_countdown_component_position(const Napi::Object& options);
    static CountdownComponentStyle    parse_countdown_component_style(const Napi::Object& options);
    static std::vector<int>           parse_countdown_moment_with_number(const Napi::Object& options);
    static CountdownOptions           parse_countdown_options(const Napi::Object& options);

    static std::vector<u_char>        hexadecimal_color_to_argb(const std::string& hex);
    static std::vector<int>           minus_one_second_to_duration(const std::vector<int>& duration);

    //
    // Internal instance of an image object
    //

    // Internal image object
    vips::VImage image_;

    // What the image_ is read from a file, this is the path
    std::string imageOriginalPath_;

    // Image mode, either IMAGE or COUNTDOWN
    ImageMode mode_;

    // Countdown animation generation options
    CountdownOptions countdownOptions_;
    // Countdown animation cache for performance
    std::vector<vips::VImage> countdownDigits_;
};

#endif
