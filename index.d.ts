export type HexadecimalColor = `#${string}`;

export type ComponentPosition = "top" | "bottom" | "left" | "right" | "center" | "top-left" | "top-right" | "bottom-left" | "bottom-right";

export type Dimension2D<T> = {
    x: T;
    y: T;
}

export type Position2D = Dimension2D<number> & {
    width?: number;
    height?: number;
};

export declare type CreationOptions = {
  width: number;
  height: number;
  bgColor: HexadecimalColor;
};

export declare type DrawTextOptions = {
  font?: string;
  fontFile?: string;
  color?: string;
  align?: "center" | "left" | "right";
  baseline?: "top" | "middle" | "bottom";
};

export type CountdownMoment<T> = {
    days: T;
    hours: T;
    minutes: T;
    seconds: T;
}

export type CountdownComponentPosition = {
    position: Position2D;
};

export type CountdownComponentStyle = {
  color: HexadecimalColor;
  textAlignment?: ComponentPosition;
  font?: string;
  fontFile?: string;
}

export type CountdownComponent = CountdownComponentPosition & CountdownComponentStyle & {
    text: string;
};

export type CountdownOptions = CreationOptions & {
    name: string;
    langs: string[];
    labels: Record<string, CountdownComponent>;
    digits: {
      positions: CountdownMoment<CountdownComponentPosition>
      style: CountdownComponentStyle
    };
};

export declare class NativeImage {
  constructor(filePath: string);

  static createSRGBImage(opts: CreationOptions): NativeImage;

  //
  // Countdown banner functions
  //
  static createCountdownAnimation(opts: CountdownOptions): NativeImage;
  renderCountdownAnimation(start: CountdownMoment<number>, frames: number): Buffer | string;

  static countdown(opts: CountdownOptions): number;

  drawText(text: string, topX: number, topY: number, opts?: DrawTextOptions): number;

  save(outFilePath: string): number;

}
