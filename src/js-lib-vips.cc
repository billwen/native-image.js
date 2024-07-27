#include <napi.h>
#include <vips/vips8>
#include "js-lib-vips.h"


/**
 * Added Libvips interface
*/
Napi::Object js_lib_vips::InitLibVips(Napi::Env env, Napi::Object exports) {

    if (VIPS_INIT ("js-lib-vips")) 
        vips_error_exit (NULL);

    return exports;
}
