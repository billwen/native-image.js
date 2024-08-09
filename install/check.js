// Copyright 2013 Lovell Fuller and others.
// SPDX-License-Identifier: Apache-2.0

'use strict';

const detectLibc = require('detect-libc');

const runtimeLibc = () => detectLibc.isNonGlibcLinuxSync() ? detectLibc.familySync() : '';

const runtimePlatformArch = () => `${process.platform}${runtimeLibc()}-${process.arch}`;

const isEmscripten = () => {
    const { CC } = process.env;
    return Boolean(CC && CC.endsWith('/emcc'));
};

const buildPlatformArch = () => {
    if (isEmscripten()) {
      return 'wasm32';
    }
    /* eslint camelcase: ["error", { allow: ["^npm_config_"] }] */
    const { npm_config_arch, npm_config_platform, npm_config_libc } = process.env;
    const libc = typeof npm_config_libc === 'string' ? npm_config_libc : runtimeLibc();
    return `${npm_config_platform || process.platform}${libc}-${npm_config_arch || process.arch}`;
};

module.exports = {
    buildPlatformArch
};
