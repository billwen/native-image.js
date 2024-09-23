/// <reference path="./index.d.ts" />
const { buildPlatformArch } = require('./install/check')

const runtimePlatform = buildPlatformArch();
const ni = require(`./build/Release/native-image-${runtimePlatform}.node`);

module.exports = ni
