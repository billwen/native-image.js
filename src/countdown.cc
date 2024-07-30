#include "js-lib-vips.h"

std::string js_lib_vips::countdown(const CountdownOptions& opts) {

    VImage image = VImage::black(opts.width, opts.height);
    image.add();
    return "Going to generate countdown";
}
