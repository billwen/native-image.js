const path = require("node:path");
const fs = require("node:fs");
const vips = require("../../index.js");

console.log('js-lib-vips', vips);

// Prepare output folder
const outputFolder = "../../output";
const outputFolderPath = path.resolve(__dirname, outputFolder);
if (!fs.existsSync(outputFolderPath)) {
  fs.mkdirSync(outputFolderPath);
}

const outputFileName = "countdown.gif";
const outputFilePath = path.resolve(outputFolderPath, outputFileName);

const countdownOptions = {
    width: 100,
    height: 100,
    bgColor: "#616161",
    outFilePath: outputFilePath
}

console.log("Testing countdown: ", vips.countdown(countdownOptions));

const image = vips.NativeImage.createSRGBImage(countdownOptions);
