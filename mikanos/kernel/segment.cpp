// day08b
#include "segment.hpp"
#include "asmfunc.h"

/** @brief グローバルディスクリプタテーブル（GDT） */
namespace {
  std::array<SegmentDescriptor, 3> gdt;
}

/**
 * SetCodeSegment
 *   コードセグメントディスクリプタを設定する
 *   実行可能なセグメントのことをコードセグメントと呼ぶ
 *
 * @param desc                        設定対象のディスクリプタ
 * @param type                        ディスクリプタタイプ（実行/読み取りなど）
 * @param descriptor_privilege_level  特権レベル（0~3） DPLとも呼ぶ
 * @param base                        セグメントのベースアドレス
 * @param limit                       セグメントのリミット
 */
void SetCodeSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit) {
  desc.data = 0;

  desc.bits.base_low = base & 0xffffu;
  desc.bits.base_middle = (base >> 16) & 0xffu;
  desc.bits.base_high = (base >> 24) & 0xffu;

  desc.bits.limit_low = limit & 0xffffu;
  desc.bits.limit_high = (limit >> 16) & 0xfu;

  desc.bits.type = type;
  desc.bits.system_segment = 1; // 1: コード/データセグメント
  desc.bits.descriptor_privilege_level = descriptor_privilege_level;
  desc.bits.present = 1;
  desc.bits.available = 0;
  desc.bits.long_mode = 1;
  desc.bits.default_operation_size = 0; // long_mode=1の時は0
  desc.bits.granularity = 1;
}

/**
 * SetDataSegment
 *   データセグメントディスクリプタを設定する
 *   実行不能なセグメントのことをデータセグメントという
 *
 * @param desc                        設定対象のディスクリプタ
 * @param type                        ディスクリプタタイプ（読み書きなど）
 * @param descriptor_privilege_level  特権レベル（0~3） DPLとも呼ぶ
 * @param base                        セグメントのベースアドレス
 * @param limit                       セグメントのリミット
 */
void SetDataSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit) {
  SetCodeSegment(desc, type, descriptor_privilege_level, base, limit);
  desc.bits.long_mode = 0;
  desc.bits.default_operation_size = 1; // 32ビットスタックセグメント
}

/**
 * SetupSegments
 *   GDTを初期化しCPUに登録する
 * 
 *   NULLディスクリプタ、コードセグメントディスクリプタ、データセグメントディスクリプタ
 *   の3つをGDTに設定し、LoadGDT()でCPUに登録する
 *   この処理が終わるとCPUはUEFIが作った古いGDTを見なくなる
 */
void SetupSegments() {
  gdt[0].data = 0;
  SetCodeSegment(gdt[1], DescriptorType::kExecuteRead, 0, 0, 0xfffff);
  SetDataSegment(gdt[2], DescriptorType::kReadWrite, 0, 0, 0xfffff);
  LoadGDT(sizeof(gdt) - 1, reinterpret_cast<uintptr_t>(&gdt[0]));
}