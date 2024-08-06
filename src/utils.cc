#include <memory>
#include <string>
#include <stdexcept>
#include <cstdio>
#include <vips/vips8>

#include "utils.h"

const std::string component_positions[] {"center", "top", "right", "bottom", "left", "top-right", "bottom-right", "bottom-left", "top-left" };

VipsCompassDirection jsvips::to_compass_direction(const std::string& position, const VipsCompassDirection default_position) {
    int total = array_size(component_positions);
    for (int i = 0; i < total; i++) {
        if (position == component_positions[i]) {
            return VipsCompassDirection(i);
        }
    }
    return default_position;
}
