export type TextElementOption = {
    text: string;
    fontFile?: string;
    color: number[];
    bgColor: number[];
    containerWidth: number;
    containerHeight: number;
    offsetTop?: number;
    offsetLeft?: number;
    cacheIndex?: number;
};

export type TextImageOption = {
    width: number;
    height: number;
    bgColor: number[];
    texts: TextElementOption[];
};

export type AnimationOption = {
    delay: number | number[];
    repeat: number;
};

export declare class NativeImage {
  constructor(filePath: string);

  static newTextImage(opts: TextImageOption): NativeImage;
  rebuildTextElementCache(texts: TextElementOption[]): number;
  rebuildTextElementCache2(texts: TextElementOption[], trimLeftWidth: number): number;

  animation(frames: TextElementOption[][], opt?: AnimationOption): Buffer;
  animation(frames: TextElementOption[][], filePath: string, opt?: AnimationOption): string;

  addTextElements(texts: TextElementOption[]): number;

  encode(format: string): Buffer;
  save(outFilePath: string): string;
}
