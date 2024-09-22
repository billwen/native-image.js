import fs from 'node:fs';
import path from 'node:path';
import {
    CountdownOptions,
    HexadecimalColor,
    NativeImage,
    TextElement,
    TextImageOptions
} from '../../index';

// Prepare output folder
const outputFolder = "../../output";
const outputFolderPath = path.resolve(__dirname, outputFolder);
if (!fs.existsSync(outputFolderPath)) {
  fs.mkdirSync(outputFolderPath);
}

const outputFileName = "countdown-3.gif";
const outputFilePath: string = path.resolve(outputFolderPath, outputFileName);
const textImageOutputFilePath = path.resolve(outputFolderPath, "text-image.png");

const labelColor: HexadecimalColor = "#ffffff";
const digitColor: HexadecimalColor = "#ffffff";

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

// https://docs.gtk.org/Pango/pango_markup.html
// New test
const days: TextElement = {
    text: "<span size='16pt' face='Noto IKEA Latin'>days</span>",
    fontFile: fontRegularFile,
    color: [255, 255, 255],
    bgColor: [204, 0, 8],
    containerHeight: 30,
    containerWidth: 60,
    offsetTop: 40
}

const hrs: TextElement = {
    ...days,
    text: "<span size='16pt' face='Noto IKEA Latin'>hrs</span>",
    offsetLeft: 71,
    offsetTop: 38
}

const min: TextElement = {
    ...days,
    text: "<span size='16pt' face='Noto IKEA Latin'>min</span>",
    offsetLeft: 142,
    offsetTop: 38
}

const sec: TextElement = {
    ...days,
    text: "<span size='16pt' face='Noto IKEA Latin'>sec</span>",
    offsetLeft: 213
}

const dd: TextElement = {
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>01</span>",
    fontFile: fontBoldFile,
    color: [255, 255, 255],
    bgColor: [204, 0, 8],
    containerHeight: 40,
    containerWidth: 60,
}

const hh: TextElement = {
    ...dd,
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>13</span>",
    offsetLeft: 71
};

const mm: TextElement = {
    ...dd,
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>24</span>",
    offsetLeft: 142
};

const ss: TextElement = {
    ...dd,
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>58</span>",
    offsetLeft: 213
};

const tiOptions: TextImageOptions  = {
    width: 273,
    height: 71,
    bgColor: [204, 0, 8],
    texts: [days, hrs, min, sec, dd, hh, mm, ss]
};

const options4: TextImageOptions = {
    ...tiOptions,
    texts: [days, hrs, min, sec]
};

const start2 = Date.now();
const template2 = NativeImage.newTextImage(tiOptions);
const pt2 = Date.now() - start;
console.log(`Processing time of new Text image ${pt2} ms`);

// 0 - 99 array
const arr = Array.from(Array(100).keys());
const digits = arr.map((i) => `00${i}`.slice(-2));

const elements = digits.map((i) => `<span size='32pt' face='Noto IKEA Latin' weight='bold'>${i}</span>`)
    .map((i) => ({
        ...dd,
        text: i
    }));

const elements2 = digits.map((i) => `<span size='32pt' face='Noto IKEA Latin' weight='bold'>0${i}</span>`)
    .map((i) => ({
        ...dd,
        bgColor: [204, 204, 8],
        text: i
    }));

const start3 = Date.now();
template2.rebuildTextElementCache(elements);
const pt3 = Date.now() - start3;
console.log(`Processing time of rebuild Text image ${pt3} ms`);

template2.rebuildTextElementCache2(elements2, 16);
template2.save(textImageOutputFilePath);

const Output4FilePath = path.resolve(outputFolderPath, "output4.png");

const template4 = NativeImage.newTextImage(options4);
template4.rebuildTextElementCache2(elements2, 18);

// convert html hex color to 3 integers
// const hexToRgb = (hex: string): number[] => {
//     return hex.match(/[A-Za-z0-9]{2}/g).map((i) => parseInt(i, 16));
// }


const dd4: TextElement = {
    ...dd,
    cacheIndex: 1
}

const hh4: TextElement = {
    ...hh,
    cacheIndex: 13
};

const mm4: TextElement = {
    ...mm,
    cacheIndex: 24
};

const ss4: TextElement = {
    ...ss,
    cacheIndex: 58
};

const start4 = Date.now();
template4.addTextElements([dd4, hh4, mm4, ss4 ]);
const pt4 = Date.now() - start4;
console.log(`Processing time of add Text image ${pt4} ms`);

template4.save(Output4FilePath);

// Generate a 60 frames countdown animation
const countdown2FilePath = path.resolve(outputFolderPath, "countdown_2.gif");

// Define styles
const redStyle: TextImageOptions = {
    width: 273,
    height: 71,
    bgColor: [204, 0, 8],
    texts: [days, hrs, min, sec]
};

const redStyleDigitStyle: TextElement = {
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>00</span>",
    fontFile: fontBoldFile,
    color: [255, 255, 255],
    bgColor: [204, 0, 8],
    containerHeight: 40,
    containerWidth: 60,
};

const digitElements = Array.from(["days", "hours", "minutes", "seconds"]);
const digitsStyle = {
    days: {
        ...redStyleDigitStyle,
        offsetTop: 0,
        offsetLeft: 0
    },
    hours: {
        ...redStyleDigitStyle,
        offsetTop: 0,
        offsetLeft: 71
    },
    minutes: {
        ...redStyleDigitStyle,
        offsetTop: 0,
        offsetLeft: 142
    },
    seconds: {
        ...redStyleDigitStyle,
        offsetTop: 0,
        offsetLeft: 213
    }
}

const redStyleDigits: TextElement[] = Array.from(Array(100).keys()).map((i) => ({
    ...redStyleDigitStyle,
    text: `<span size='32pt' face='Noto IKEA Latin' weight='bold'>${`000${i}`.slice(-3)}</span>`
}));

//
// Return
function secondsToCountdownElements(secs: number): {days: number, hours: number, minutes: number, seconds: number} {
    const days = Math.floor(secs / (3600 * 24));
    const hours = Math.floor(secs % (3600 * 24) / 3600);
    const minutes = Math.floor(secs % 3600 / 60);
    const seconds = Math.floor(secs % 60);

    return {
        days,
        hours,
        minutes,
        seconds
    };
}

const endDateTime = new Date("2024-10-01T00:00:00.000Z");
console.log(`End date time: ${endDateTime}`);
const now = new Date();
const diff = endDateTime.getTime() - now.getTime();
const seconds = Math.floor(diff / 1000);


const framesData = Array.from(Array(60).keys()).map<TextElement[]>((i) => {
    const countdownElements = secondsToCountdownElements(seconds - i);
    return digitElements.map<TextElement>((key) => {
        const k = key as keyof typeof countdownElements;
        const digit = countdownElements[k] ?? 0;
        return {
            ...digitsStyle[k],
            text: `<span size='32pt' face='Noto IKEA Latin' weight='bold'>${`000${digit}`.slice(-3)}</span>`,
            cacheIndex: digit
        };
    });
});

// Prepare the template
console.log(`Start generate countdown animation ${framesData.length} frames - ${countdown2FilePath}`);
const redStyleTemplate = NativeImage.newTextImage(redStyle);
redStyleTemplate.rebuildTextElementCache2(redStyleDigits, 18);

// Start generate
const start5 = Date.now();
redStyleTemplate.animation(framesData,countdown2FilePath);
const pt5 = Date.now() - start5;
console.log(`Processing time of countdown animation ${pt5} ms - ${framesData.length} frames - ${countdown2FilePath}`);

