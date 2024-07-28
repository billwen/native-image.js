const { buildPlatformArch } = require('./install/check')

const runtimePlatform = buildPlatformArch();

const vips = require('bindings')(`js-lib-vips-${runtimePlatform}.node`);

module.export = vips
