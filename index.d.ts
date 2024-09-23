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

export declare class NativeImage {
  constructor(filePath: string);

  static newTextImage(opts: TextImageOption): NativeImage;
  rebuildTextElementCache(texts: TextElementOption[]): number;
  rebuildTextElementCache2(texts: TextElementOption[], trimLeftWidth: number): number;

  animation(frames: TextElementOption[][]): Buffer;
  animation(frames: TextElementOption[][], filePath: string): string;

  addTextElements(texts: TextElementOption[]): number;

  encode(format: string): Buffer;
  save(outFilePath: string): string;
}
