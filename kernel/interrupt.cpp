/**
 * @file interrupt.cpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */
// day07a
#include "interrupt.hpp"

/** @brief 割り込み記述子テーブル（IDT）本体 */
std::array<InterruptDescriptor, 256> idt;

/**
 * SetIDTEntry:
 *    IDTの割り込み記述子に値を設定する
 *
 * @param desc              : 設定対象の割り込み記述子
 * @param attr              : 割り込み記述子の属性
 * @param offset            : 割り込みハンドラのアドレス
 * @param segment_selector  : コードセグメントセレクタ
 */
void SetIDTEntry(InterruptDescriptor& desc,
                 InterruptDescriptorAttribute attr,
                 uint64_t offset,
                 uint16_t segment_selector) {
  desc.attr = attr;
  desc.offset_low = offset & 0xffffu;
  desc.offset_middle = (offset >> 16) & 0xffffu;
  desc.offset_high = offset >> 32;
  desc.segment_selector = segment_selector;
}

/**
 * NotifyEndOfInterrupt:
 *    割り込みハンドラの処理が完了したことをCPUに知らせる
 *    これにより、CPUは一時停止していた割り込みの受付を再開できる
 */
void NotifyEndOfInterrupt() {
  volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
  *end_of_interrupt = 0;
}