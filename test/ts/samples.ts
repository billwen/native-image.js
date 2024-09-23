import path from "node:path";
import fs from "node:fs";
import {format} from "node:util";
import {NativeImage, TextElementOption, TextImageOption} from "../../index";

//
// Paths
//
export const ikeaFontRegularFile = path.resolve(__dirname, "../../output/fonts/NotoIKEALatin-Regular.ttf");
export const ikeaFontBoldFile = path.resolve(__dirname, "../../output/fonts/NotoIKEALatin-Bold.ttf");

const outputFolder = "../../output";

export function outputFolderPath(): string {
    const output = path.resolve(__dirname, outputFolder);
    if (!fs.existsSync(output)) {
        fs.mkdirSync(output, { recursive: true });
    }

    return output;
}

//
// Help Functions
//
export function secondsToCountdownElements(secs: number): {days: number, hours: number, minutes: number, seconds: number} {
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

//
// The "RED" style
//
export const redLabelDays: TextElementOption = {
    text: "<span size='16pt' face='Noto IKEA Latin'>days</span>",
    fontFile: ikeaFontRegularFile,
    color: [255, 255, 255],
    bgColor: [204, 0, 8],
    containerHeight: 30,
    containerWidth: 60,
    offsetTop: 40
}

export const redLabelHrs: TextElementOption = {
    ...redLabelDays,
    text: "<span size='16pt' face='Noto IKEA Latin'>hrs</span>",
    offsetLeft: 71,
    offsetTop: 38
}

export const redLabelMin: TextElementOption = {
    ...redLabelDays,
    text: "<span size='16pt' face='Noto IKEA Latin'>mins</span>",
    offsetLeft: 142,
    offsetTop: 38
}

export const redLabelSec: TextElementOption = {
    ...redLabelDays,
    text: "<span size='16pt' face='Noto IKEA Latin'>secs</span>",
    offsetLeft: 213
}

export const redDigitDays: TextElementOption = {
    text: "<span size='32pt' face='Noto IKEA Latin' weight='bold'>%s</span>",
    fontFile: ikeaFontBoldFile,
    color: [255, 255, 255],
    bgColor: [204, 0, 8],
    containerHeight: 40,
    containerWidth: 60,
}

export const redDigitHrs: TextElementOption = {
    ...redDigitDays,
    offsetLeft: 71
};

export const redDigitMin: TextElementOption = {
    ...redDigitDays,
    offsetLeft: 142
};

export const redDigitSec: TextElementOption = {
    ...redDigitDays,
    offsetLeft: 213
};

const redStyleOptions: TextImageOption  = {
    width: 273,
    height: 71,
    bgColor: [204, 0, 8],
    texts: [redLabelDays, redLabelHrs, redLabelMin, redLabelSec]
};

export function drawRedCountdown(endDateTime: string, outputFileName: string): void {
    const endDate = new Date(endDateTime);
    if (isNaN(endDate.getTime())) {
        throw new Error(`Invalid endDateTime ${endDateTime}.`);
    }

    const start = Date.now();
    const leftSeconds = Math.floor((endDate.getTime() - start) / 1000);
    if (leftSeconds <= 0) {
        throw new Error(`Invalid endDateTime ${endDateTime}, should before ${start}.`);
    }

    // Prepare the countdown digits
    const digitsCache = Array.from(Array(100).keys()).map((i) => {
        const digits = format(redDigitDays.text, `000${i}`.slice(-3));
        return {
            ...redDigitDays,
            text: format(digits)
        };
    });

    // Prepare the countdown frames
    const totalFrames = leftSeconds < 60 ? leftSeconds + 1 : 60;
    const frameOptions = Array.from(Array(totalFrames).keys()).map<TextElementOption[]>((i) => {
        const {days, hours, minutes, seconds} = secondsToCountdownElements(leftSeconds - i);
        return [
            {
                ...redDigitDays,
                text: format(redDigitDays.text, `00${days}`.slice(-2)),
                cacheIndex: days
            },
            {
                ...redDigitHrs,
                text: format(redDigitHrs.text, `00${hours}`.slice(-2)),
                cacheIndex: hours
            },
            {
                ...redDigitMin,
                text: format(redDigitMin.text, `00${minutes}`.slice(-2)),
                cacheIndex: minutes
            },
            {
                ...redDigitSec,
                text: format(redDigitSec.text, `00${seconds}`.slice(-2)),
                cacheIndex: seconds
            }];
    });

    // Draw the template
    const redTemplate = NativeImage.newTextImage(redStyleOptions);

    // Prepare the countdown elements
    redTemplate.rebuildTextElementCache2(digitsCache, 18);

    const pt1 = Date.now();
    redTemplate.animation(frameOptions, path.resolve(outputFolderPath(), outputFileName));
    const pt = Date.now() - pt1;
    console.log(`Generate ${outputFileName} containing ${totalFrames} frames in ${pt}ms.`);
}
