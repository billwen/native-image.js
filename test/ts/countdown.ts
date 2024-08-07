import fs from 'node:fs';
import path from 'node:path';
import {CountdownOptions, HexadecimalColor, NativeImage} from '../../index';

// Prepare output folder
const outputFolder = "../../output";
const outputFolderPath = path.resolve(__dirname, outputFolder);
if (!fs.existsSync(outputFolderPath)) {
  fs.mkdirSync(outputFolderPath);
}

const outputFileName = "countdown-2.gif";
const outputFilePath: string = path.resolve(outputFolderPath, outputFileName);

const labelColor: HexadecimalColor = "#ffffff";
const digitColor: HexadecimalColor = "#ffffff";
const labelFont = "Noto IKEA Latin Regular 16";
const digitFont = "Noto IKEA Latin Bold 32";
const fontFile = path.resolve(__dirname, "../../output/fonts/DancingScript-VariableFont_wght.ttf");

const countdownOptions: CountdownOptions = {
    name: "red-v1-en",
    width: 273,
    height: 71,
    bgColor: "#cc0008",
    langs: ["en"],
    labels: {
        days: {
            text: "days",
            position: {x: 0, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            font: labelFont,
            fontFile
        },
        hours: {
            text: "hrs",
            position: {x: 71, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            font: labelFont,
            fontFile
        },
        minutes: {
            text: "min",
            position: {x: 142, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            font: labelFont,
            fontFile
        },
        seconds: {
            text: "sec",
            position: {x: 213, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            font: labelFont,
            fontFile
        }
    },
    digits: {
      positions: {
        days: {
          position: {x: 0, y: 0, width: 60, height: 40},
        },
        hours: {
          position: {x: 71, y: 0, width: 60, height: 40},
        },
        minutes: {
          position: {x: 142, y: 0, width: 60, height: 40},
        },
        seconds: {
          position: {x: 213, y: 0, width: 60, height: 40},
        }
      },
      style: {
        color: digitColor,
        textAlignment: "center",
        font: digitFont,
        fontFile
      }

    }
}

// const image = new NativeImage();
const emptyImage = NativeImage.createCountdownAnimation(countdownOptions);
emptyImage.save(outputFilePath);

