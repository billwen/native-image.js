const vips = require('./build/Release/js-lib-vips.node')

console.log('js-lib-vips', vips);
console.log("result", vips.hello());
console.log("add", vips.add(12,13));

module.export = vips
