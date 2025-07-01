// day04a
#pragma once

#include <stdint.h>

/*
  PixelFormat: 
    UEFIの規格で定められたピクセルのデータ形式
*/
enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
};

/*
  FrameBufferConfig: 
    UEFIの機能を使って取得したGOPを保持する構造体
*/
struct FrameBufferConfig {
  uint8_t* frame_buffer;
  uint32_t pixels_per_scan_line;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};