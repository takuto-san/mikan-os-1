/**
 * @file graphics.cpp
 *
 * 画像描画関連のプログラムを集めたファイル．
 */
// day05b

#include "graphics.hpp"

/** 
 * RGB(BGR)Resv8BitPerColorPixelWriter: 
 *    ピクセルのデータ形式にしたがって実際に描画する部分
 */
// day04b, day04c
void RGBResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  p[0] = c.r;
  p[1] = c.g;
  p[2] = c.b;
}

void BGRResv8BitPerColorPixelWriter::Write(int x, int y, const PixelColor& c) {
  auto p = PixelAt(x, y);
  p[0] = c.b;
  p[1] = c.g;
  p[2] = c.r;
}

/** 
 * DrawRectangle: 
 *    長方形を塗りつぶす関数
 * 
 * @param : 第2引数から順に、長方形の左上の座標、長方形の大きさ、描画色
 */
// day06a
void DrawRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& c) {
  for (int dx = 0; dx < size.x; ++dx) {
    writer.Write(pos.x + dx, pos.y, c);
    writer.Write(pos.x + dx, pos.y + size.y - 1, c);
  }
  for (int dy = 1; dy < size.y - 1; ++dy) {
    writer.Write(pos.x, pos.y + dy, c);
    writer.Write(pos.x + size.x - 1, pos.y + dy, c);
  }
}

/** 
 * FillRectangle: 
 *  長方形の枠だけを描く関数
 *  @param : 第2引数から順に、長方形の左上の座標、長方形の大きさ、描画色
 */
void FillRectangle(PixelWriter& writer, const Vector2D<int>& pos,
                   const Vector2D<int>& size, const PixelColor& c) {
  for (int dy = 0; dy < size.y; ++dy) {
    for (int dx = 0; dx < size.x; ++dx) {
      writer.Write(pos.x + dx, pos.y + dy, c);
    }
  }
}