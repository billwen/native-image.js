export declare function countdown(options: CountdownOptions): void;

export declare type CreationOptions = {
  width: number;
  height: number;
  bgColor?: string;
};

export declare type DrawTextOptions = {
  font?: string;
  fontFile?: string;
  color?: string;
  align?: "center" | "left" | "right";
  baseline?: "top" | "middle" | "bottom";
};

export declare type CountdownOptions = CreationOptions & {
  outFilePath: string;
};

export declare class NativeImage {
  constructor(filePath: string);

  static createSRGBImage(opts: CreationOptions): NativeImage;

  drawText(text: string, topX: number, topY: number, opts?: DrawTextOptions): number;

  save(outFilePath: string): number;

  countdown(): string;
}
