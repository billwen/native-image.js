const path = require("node:path");
const fs = require("node:fs");
const vips = require("../../index.js");
const { NativeImage } = require("../../index.js");

console.log('js-lib-vips', vips);

// Prepare output folder
const outputFolder = "../../output";
const outputFolderPath = path.resolve(__dirname, outputFolder);
if (!fs.existsSync(outputFolderPath)) {
  fs.mkdirSync(outputFolderPath);
}

const outputFileName = "countdown.gif";
const outputFilePath = path.resolve(outputFolderPath, outputFileName);

const fontFilePath = path.resolve(__dirname, "../../output/fonts/DancingScript-VariableFont_wght.ttf");
const font = 'Dancing Script bold 128px';

const countdownOptions = {
    width: 400,
    height: 100,
    bgColor: "#667788",
    outFilePath: outputFilePath
}

const start = Date.now();
console.log("Testing countdown: ", vips.NativeImage.countdown(countdownOptions));
const pt = Date.now() - start;
console.log("countdown took: ", pt);

const image = vips.NativeImage.createSRGBImage(countdownOptions);

const textOptions = {
    font,
    fontFile: fontFilePath,
    color: "#0f0",
}

image.drawText("Hello, libVips!", 100, 20, textOptions);

image.save(outputFilePath);
