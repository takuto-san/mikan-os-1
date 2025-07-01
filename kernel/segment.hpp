/**
 * @file segment.hpp
 *
 * セグメンテーション関連のプログラムを集めたファイル
 */
// day08b
#pragma once
#include <array>
#include <cstdint>
#include "x86_descriptor.hpp"

/**
 * SegmentDescriptor
 *   GDTの個々の要素である8バイトのセグメントディスクリプタを定義する
 */
union SegmentDescriptor {
  uint64_t data;
  struct {
    uint64_t limit_low : 16;
    uint64_t base_low : 16;
    uint64_t base_middle : 8;
    DescriptorType type : 4;
    uint64_t system_segment : 1;             // 1:コード/データ, 0:システム
    uint64_t descriptor_privilege_level : 2; // DPL (特権レベル 0~3)
    uint64_t present : 1;                    // P (セグメントがメモリ上に存在)
    uint64_t limit_high : 4;
    uint64_t available : 1;                  // AVL (OSが自由に利用可)
    uint64_t long_mode : 1;                  // L (64ビットモード)
    uint64_t default_operation_size : 1;     // D/B (0:16bit, 1:32bit)
    uint64_t granularity : 1;                // G (0:1B, 1:4KiB単位)
    uint64_t base_high : 8;
  } __attribute__((packed)) bits;
} __attribute__((packed));

/** @brief コードセグメントディスクリプタを設定する */
void SetCodeSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit);

/** @brief データセグメントディスクリプタを設定する */
void SetDataSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit);

/** @brief GDTを初期化しCPUに登録する */
void SetupSegments();