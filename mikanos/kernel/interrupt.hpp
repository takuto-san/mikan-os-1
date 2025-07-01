/**
 * @file interrupt.hpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */
// day07a
#pragma once
#include <array>
#include <cstdint>
// day08a
#include "x86_descriptor.hpp"

/** @brief IDT（割り込み記述子テーブル: 割り込みベクタと割り込みハンドラの対応表）の定義 */
/**
 * InterruptDescriptorAttribute:
 *   割り込み記述子のx属性を表現する
 */
union InterruptDescriptorAttribute {
  uint16_t data;
  struct {
    uint16_t interrupt_stack_table : 3;
    uint16_t : 5;
    DescriptorType type : 4;
    uint16_t : 1;
    uint16_t descriptor_privilege_level : 2;
    uint16_t present : 1;
  } __attribute__((packed)) bits;
} __attribute__((packed));

/**
 * InterruptDescriptor:
 *   割り込みハンドラのアドレスや属性を定義する、割り込み記述子
 */
struct InterruptDescriptor {
  uint16_t offset_low;
  uint16_t segment_selector;
  InterruptDescriptorAttribute attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t reserved;
} __attribute__((packed));

extern std::array<InterruptDescriptor, 256> idt;

/**
 * MakeIDTAttr:
 *   割り込み記述子の属性を生成するヘルパー関数
 *
 * @param type                       : 記述子の種別
 * @param descriptor_privilege_level : 記述子権限レベル（DPL）
 * @param present                    : ディスクリプタが有効か（メモリ上に存在するか）
 * @param interrupt_stack_table      : 割り込みスタックテーブル（IST）のインデックス
 * @return                           : 生成された割り込み記述子の属性
 */
constexpr InterruptDescriptorAttribute MakeIDTAttr(
    DescriptorType type,
    uint8_t descriptor_privilege_level,
    bool present = true,
    uint8_t interrupt_stack_table = 0) {
  InterruptDescriptorAttribute attr{};
  attr.bits.interrupt_stack_table = interrupt_stack_table;
  attr.bits.type = type;
  attr.bits.descriptor_privilege_level = descriptor_privilege_level;
  attr.bits.present = present;
  return attr;
}

void SetIDTEntry(InterruptDescriptor& desc,
                 InterruptDescriptorAttribute attr,
                 uint64_t offset,
                 uint16_t segment_selector);

/** @brief 特定の割り込みベクタ番号（割り込みID）を定数として定義する */
class InterruptVector {
 public:
  enum Number {
    kXHCI = 0x40,
  };
};

/**
 * InterruptFrame:
 *   割り込み発生時にスタックに積まれるCPUのレジスタ情報
 *   割り込みハンドラは、この構造体へのポインタを引数として受け取ることで、
 *   割り込み発生時のCPUの状態（どのプログラムを実行していたか等）を知ることができる．
 */
struct InterruptFrame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void NotifyEndOfInterrupt();