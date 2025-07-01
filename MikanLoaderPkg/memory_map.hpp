#pragma once

#include <stdint.h>

/**
 * MemoryMap
 *   UEFIのメモリマップ情報を格納する構造体
 *   UEFIのGetMemoryMap()サービスが返す情報をそのまま保持する
 */
struct MemoryMap {
  unsigned long long buffer_size;
  void* buffer;
  unsigned long long map_size;
  unsigned long long map_key;
  unsigned long long descriptor_size;
  uint32_t descriptor_version;
};

/**
 * MemoryDescriptor
 *   メモリ領域の情報を表す構造体
 *   UEFI仕様におけるメモリディスクリプタの定義にほぼ準ずる
 */
struct MemoryDescriptor {
  uint32_t type;
  uintptr_t physical_start;
  uintptr_t virtual_start;
  uint64_t number_of_pages;
  uint64_t attribute;
};

/**
 * MemoryType
 *   メモリディスクリプタで利用されるメモリ種別。
 *   UEFI仕様で定義されているメモリタイプに対応する。
 */
#ifdef __cplusplus
enum class MemoryType {
  kEfiReservedMemoryType,
  kEfiLoaderCode,
  kEfiLoaderData,
  kEfiBootServicesCode,
  kEfiBootServicesData,
  kEfiRuntimeServicesCode,
  kEfiRuntimeServicesData,
  kEfiConventionalMemory,
  kEfiUnusableMemory,
  kEfiACPIReclaimMemory,
  kEfiACPIMemoryNVS,
  kEfiMemoryMappedIO,
  kEfiMemoryMappedIOPortSpace,
  kEfiPalCode,
  kEfiPersistentMemory,
  kEfiMaxMemoryType
};

/** @brief uint32_tとMemoryTypeを比較するための演算子オーバーロード */
inline bool operator==(uint32_t lhs, MemoryType rhs) {
  return lhs == static_cast<uint32_t>(rhs);
}

/** @brief MemoryTypeとuint32_tを比較するための演算子オーバーロード */
inline bool operator==(MemoryType lhs, uint32_t rhs) {
  return rhs == lhs;
}
#endif
