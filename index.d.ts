export declare function countdown(options: CountdownOptions): void;

export declare type CreationOptions = {
  width: number;
  height: number;
  bgColor: string;
};

export declare type CountdownOptions = CreationOptions & {
  outFilePath: string;
};

export declare class NativeImage {
  constructor(filePath: string);

  static createSRGBImage(opts: CreationOptions): NativeImage;

  countdown(): string;
}
