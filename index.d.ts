export declare function countdown(options: CountdownOptions): void;

export declare type CountdownOptions = {
  width: number;
  height: number;
  bgColor: string;
  outFilePath: string;
};

export declare class NativeImage {
  constructor();

  static createImage(width: number, height: number, bgColor?: string): NativeImage;

  countdown(): string;
}
