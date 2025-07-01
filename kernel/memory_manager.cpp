// day08c
#include "memory_manager.hpp"

/** @brief ビットマップ方式のメモリ管理クラスのコンストラクタ */
BitmapMemoryManager::BitmapMemoryManager()
  : alloc_map_{}, 
    range_begin_{FrameID{0}}, 
    range_end_{FrameID{kFrameCount}} {
}

/**
 * Allocate
 *   要求されたフレーム数の連続したメモリ領域を確保する
 *   引数に割り当てたいページフレームの数を渡すと、連続でその大きさ以上の空きがある領域を探して割り当てる
 * 
 * @param num_frames 確保したいフレーム数
 * @return           確保したメモリ領域の先頭フレームIDとエラーコード
 */
WithError<FrameID> BitmapMemoryManager::Allocate(size_t num_frames) {
  // 検索を開始するフレームIDを管理範囲の先頭に設定
  size_t start_frame_id = range_begin_.ID();
  while (true) {
    size_t i = 0;
    for (; i < num_frames; ++i) {
      if (start_frame_id + i >= range_end_.ID()) {
        return {kNullFrame, MAKE_ERROR(Error::kNoEnoughMemory)};
      }
      if (GetBit(FrameID{start_frame_id + i})) {
        // "start_frame_id + i" にあるフレームは割り当て済み
        break;
      }
    }
    if (i == num_frames) {
      // num_frames 分の空きが見つかった
      MarkAllocated(FrameID{start_frame_id}, num_frames);
      return {
        FrameID{start_frame_id},
        MAKE_ERROR(Error::kSuccess),
      };
    }
    // 次のフレームから再検索
    start_frame_id += i + 1;
  }
}

/**
 * Free
 *   指定されたメモリ領域を解放（未使用状態に）する
 * 
 * @param start_frame 解放する領域の先頭フレームID
 * @param num_frames  解放するフレーム数
 * @return            エラーコード
 */
Error BitmapMemoryManager::Free(FrameID start_frame, size_t num_frames) {
  for (size_t i = 0; i < num_frames; ++i) {
    SetBit(FrameID{start_frame.ID() + i}, false);
  }
  return MAKE_ERROR(Error::kSuccess);
}

/**
 * MarkAllocated
 *   指定されたメモリ領域を「使用中」としてマークする
 * 
 * @param start_frame マークする領域の先頭フレームID
 * @param num_frames  マークするフレーム数
 */
void BitmapMemoryManager::MarkAllocated(FrameID start_frame, size_t num_frames) {
  for (size_t i = 0; i < num_frames; ++i) {
    SetBit(FrameID{start_frame.ID() + i}, true);
  }
}

/**
 * SetMemoryRange
 *   このメモリマネージャが管理するメモリの範囲を設定する
 * 
 * @param range_begin 管理範囲の開始フレームID
 * @param range_end   管理範囲の終了フレームID
 */
void BitmapMemoryManager::SetMemoryRange(FrameID range_begin, FrameID range_end) {
  range_begin_ = range_begin;
  range_end_ = range_end;
}

/**
 * GetBit
 *   指定したフレームIDに対応するビットマップの値（使用中か否か）を取得する
 * 
 * @param frame 確認したいフレームのID
 * @return      true: 使用中, false: 空き
 */
bool BitmapMemoryManager::GetBit(FrameID frame) const {
  auto line_index = frame.ID() / kBitsPerMapLine;
  auto bit_index = frame.ID() % kBitsPerMapLine;

  return (alloc_map_[line_index] & (static_cast<MapLineType>(1) << bit_index)) != 0;
}

/**
 * SetBit
 *   指定したフレームIDに対応するビットマップの値を設定する
 * 
 * @param frame     設定したいフレームのID
 * @param allocated true: 使用中(1)に設定, false: 空き(0)に設定
 */
void BitmapMemoryManager::SetBit(FrameID frame, bool allocated) {
  auto line_index = frame.ID() / kBitsPerMapLine;
  auto bit_index = frame.ID() % kBitsPerMapLine;

  if (allocated) {
    alloc_map_[line_index] |= (static_cast<MapLineType>(1) << bit_index);
  } else {
    alloc_map_[line_index] &= ~(static_cast<MapLineType>(1) << bit_index);
  }
}






