import fs from 'node:fs';
import path from 'node:path';
import {CountdownOptions, HexadecimalColor, NativeImage} from '../../index';

// Prepare output folder
const outputFolder = "../../output";
const outputFolderPath = path.resolve(__dirname, outputFolder);
if (!fs.existsSync(outputFolderPath)) {
  fs.mkdirSync(outputFolderPath);
}

const outputFileName = "countdown-3.gif";
const outputFilePath: string = path.resolve(outputFolderPath, outputFileName);

const labelColor: HexadecimalColor = "#ffffff";
const digitColor: HexadecimalColor = "#ffffff";
const labelFont = "Noto IKEA Latin Regular 16pt";
const digitFont = "Noto IKEA Latin Bold 32pt";
const fontRegularFile = path.resolve(__dirname, "../../output/fonts/NotoIKEALatin-Regular.ttf");
const fontBoldFile = path.resolve(__dirname, "../../output/fonts/NotoIKEALatin-Bold.ttf");
console.log(`Bold font: ${fontBoldFile} - Regular font: ${fontRegularFile}`)

const countdownOptions: CountdownOptions = {
    name: "red-v1-en",
    width: 273,
    height: 71,
    bgColor: "#cc0008",
    langs: ["en"],
    labels: {
        days: {
            text: "<span foreground='#ffffff' face='Noto IKEA Latin' weight='normal' size='16pt' >days</span>",
            position: {x: 0, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            fontFile: fontRegularFile
        },
        hours: {
            text: "<span foreground='#ffffff' face='Noto IKEA Latin' weight='normal' size='16pt' >hrs</span>",
            paddingBottom: 3,
            position: {x: 71, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            fontFile: fontRegularFile
        },
        minutes: {
            text: "<span foreground='#ffffff' face='Noto IKEA Latin' weight='normal' size='16pt' >min</span>",
            paddingTop: 1,
            paddingBottom: 4,
            position: {x: 142, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            fontFile: fontRegularFile
        },
        seconds: {
            text: "<span foreground='#ffffff' face='Noto IKEA Latin' weight='normal' size='16pt' >sec</span>",
            paddingTop: 4,
            paddingBottom: 3,
            position: {x: 213, y: 40, width: 60, height: 31},
            color: labelColor,
            textAlignment: "center",
            fontFile: fontRegularFile
        }
    },
    digits: {
      positions: {
        days: {
          position: {x: 0, y: 5},
        },
        hours: {
          position: {x: 71, y: 5},
        },
        minutes: {
          position: {x: 142, y: 5},
        },
        seconds: {
          position: {x: 213, y: 5},
        }
      },
      style: {

        color: digitColor,
        width: 60,
        height: 40,
        textAlignment: "center",
        fontFile: fontBoldFile
      },
      textTemplate: "<span foreground='#ffffff' face='Noto IKEA Latin' weight='bold' size='32pt' >%s</span>",

    }
}

// const image = new NativeImage();
const template = NativeImage.createCountdownAnimation(countdownOptions);
const start = Date.now();
template.renderCountdownAnimation({days: 1, hours: 2, minutes: 3, seconds: 4}, 60, outputFilePath);
const pt = Date.now() - start;

//emptyImage.save(outputFilePath);
console.log(`Processing time ${pt}`);

