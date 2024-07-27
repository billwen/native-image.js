const { buildPlatformArch } = require('./install/check')

const runtimePlatform = buildPlatformArch();

let vips;

try {
    vips = require(`./build/Release/js-lib-vips-${runtimePlatform}.node`);

    console.log('js-lib-vips', vips);

    console.log("result", vips.hello());
    console.log("add", vips.add(12,13));
    
    const sampleClass = new vips.ActualClass(4.3);
    console.log('Testing class initial value: ', sampleClass.getValue());
    console.log("After adding 3.3 ", sampleClass.add(3.3));

} catch (err) {
    throw new Error(err);
}




module.export = vips
