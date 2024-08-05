{
    "variables": {
        'platform_and_arch': '<!(node -p "require(\'./install/check\').buildPlatformArch()")',
        'pkg_config_path': '',
    },
    "targets": [
        {
            "target_name": "js-lib-vips-<(platform_and_arch)",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions"],
            "sources": [
                "src/main.cc",
                "src/native_image.cc"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                '<!@(PKG_CONFIG_PATH="<(pkg_config_path)" pkg-config --cflags-only-I vips-cpp vips glib-2.0 | sed s\/-I//g)'
            ],
            'libraries': [
                '<!@(PKG_CONFIG_PATH="<(pkg_config_path)" pkg-config --libs vips-cpp)'
            ],
            'dependencies': [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            "conditions": [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    }
                }]
            ]
        }
    ]
}
