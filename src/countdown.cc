#include "js-lib-vips.h"

using namespace vips;

std::string js_lib_vips::countdown(const CountdownOptions& opts) {

    VImage image = VImage::black(opts.width, opts.height);

    return "Going to generate countdown";
}
