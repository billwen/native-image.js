/// <reference path="./index.d.ts" />
const { buildPlatformArch } = require('./install/check')

const runtimePlatform = buildPlatformArch();
const ni = require('bindings')({
    bindings: `native-image-${runtimePlatform}`,
    compiled: 'build',
});

module.exports = ni
