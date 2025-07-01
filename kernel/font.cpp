// day05b
/**
 * @file font.cpp
 *
 * フォント描画のプログラムを集めたファイル.
 */

#include "font.hpp"

// day05c <- day05a
extern const uint8_t _binary_hankaku_bin_start;
extern const uint8_t _binary_hankaku_bin_end;
extern const uint8_t _binary_hankaku_bin_size;

/** 
 * GetFont: 
 *  ASCIIコードに対応したフォントデータ（1文字あたり16バイト）の先頭アドレスを返す
 *  @param : フォントを取得したい文字のASCIIコード
 *  @return: フォントデータの先頭ポインタ.対応するフォントがない場合はnullptrを返す
 */

const uint8_t* GetFont(char c) {
  auto index = 16 * static_cast<unsigned int>(c);
  if (index >= reinterpret_cast<uintptr_t>(&_binary_hankaku_bin_size)) {
    return nullptr;
  }
  return &_binary_hankaku_bin_start + index;
}

/** 
 * WriteAscii: 
 *  指定された位置に1文字分のASCII文字を描画する
 *  @param : 描画する文字のASCIIコードなど
 */

void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  const uint8_t* font = GetFont(c);
  if (font == nullptr) {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      if ((font[dy] << dx) & 0x80u) {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}

// day05d
/** 
 * WriteString: 
 *  書式なしの文字列表示に対応させる
 *  @param : 描画する文字のASCIIコードなど
 */
void WriteString(PixelWriter& writer, int x, int y, const char* s, const PixelColor& color) {
  for (int i = 0; s[i] != '\0'; ++i) {
    WriteAscii(writer, x + 8 * i, y, s[i], color);
  }
}