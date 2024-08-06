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


export type ComponentPosition = "top" | "bottom" | "left" | "right" | "center" | "top-left" | "top-right" | "bottom-left" | "bottom-right";

export type CountdownMoment<T> = {
    days: T;
    hours: T;
    minutes: T;
    seconds: T;
}

export type Dimension2D<T> = {
    x: T;
    y: T;
}

export type CountdownComponent = {
    locations: CountdownMoment<Dimension2D<number>>;
    color: string;
    size: Dimension2D<number>;
    align?: ComponentPosition;
    font?: string;
    fontFile?: string;
};

export type CountdownLabelComponent = CountdownComponent & {
    labels: CountdownMoment<string>;
};

export type CountdownOptions = CreationOptions & {
    labelComponent: CountdownLabelComponent;
    digitComponent: CountdownComponent;
};

export declare class NativeImage {
  constructor(filePath: string);

  static createSRGBImage(opts: CreationOptions): NativeImage;

  //
  // Countdown banner functions
  //
  static prepareCountdownAnimation(opts: CountdownOptions): NativeImage;
  renderCountdownAnimation(start: CountdownMoment<number>, frames: number): Buffer;

  static countdown(opts: CountdownOptions): number;

  drawText(text: string, topX: number, topY: number, opts?: DrawTextOptions): number;

  save(outFilePath: string): number;


}
