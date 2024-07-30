/// <reference path="./index.d.ts" />
const { buildPlatformArch } = require('./install/check')

const runtimePlatform = buildPlatformArch();
const vips = require('bindings')({
    bindings: `js-lib-vips-${runtimePlatform}`,
    compiled: 'build',
});

module.exports = vips
