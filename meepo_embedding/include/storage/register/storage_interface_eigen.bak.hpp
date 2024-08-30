/*Copyright 2024 The MeepoEmbedding Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#pragma once

#ifndef MEEPO_EMBEDDING_STORAGE_INTERFACE_HPP_
#define MEEPO_EMBEDDING_STORAGE_INTERFACE_HPP_

#include <expected>
#include <string>
#include <system_error>

#include "Eigen/Core"  // from @eigen
#include "meepo_embedding/include/common/device_base.h"
#include "meepo_embedding/include/common/status.h"
#include "meepo_embedding/include/third_party/proxy.h"
#include "yaml-cpp/yaml.h"  // from @yaml-cpp

namespace meepo_embedding {
namespace storage {
struct NotImplemented {
  explicit NotImplemented(auto &&...) {
    throw std::runtime_error{
      "Not implemented function in storage backend class instance!"};
  }

  template <class T>
  operator T() const noexcept {
    std::unreachable();
  }
};

//
// Specifications of abstraction
// For more details, please check:
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3086r2.pdf
//
PRO_DEF_MEM_DISPATCH(MemInit, init);
PRO_DEF_MEM_DISPATCH(MemDevice, device);
PRO_DEF_MEM_DISPATCH(MemDim, dim);
PRO_DEF_MEM_DISPATCH(MemFind, find);
PRO_DEF_MEM_DISPATCH(MemFindOrInsert, find_or_insert);
PRO_DEF_MEM_DISPATCH(MemContains, contains);
PRO_DEF_MEM_DISPATCH(MemAssign, assign);
PRO_DEF_MEM_DISPATCH(MemAssignValues, assign_values);
PRO_DEF_MEM_DISPATCH(MemAssignScores, assign_scores);
PRO_DEF_MEM_DISPATCH(MemInsertAndEvict, insert_and_evict);
PRO_DEF_MEM_DISPATCH(MemInsertOrAssign, insert_or_assign);
PRO_DEF_MEM_DISPATCH(MemAccumOrAssign, accum_or_assign);
PRO_DEF_MEM_DISPATCH(MemErase, erase);
PRO_DEF_MEM_DISPATCH(MemEraseIf, erase_if);
PRO_DEF_MEM_DISPATCH(MemClear, clear);
PRO_DEF_MEM_DISPATCH(MemExportBatch, export_batch);
PRO_DEF_MEM_DISPATCH(MemExportBatchIf, export_batch_if);
PRO_DEF_MEM_DISPATCH(MemEmpty, empty);
PRO_DEF_MEM_DISPATCH(MemSize, size);
PRO_DEF_MEM_DISPATCH(MemCapacity, capacity);
PRO_DEF_MEM_DISPATCH(MemReserve, reserve);
PRO_DEF_MEM_DISPATCH(MemSave, save);
PRO_DEF_MEM_DISPATCH(MemLoad, load);

PRO_DEF_WEAK_DISPATCH(WeakMemInit, MemInit, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemDevice, MemDevice, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemDim, MemDim, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemFind, MemFind, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemFindOrInsert, MemFindOrInsert, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemContains, MemContains, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemAssign, MemAssign, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemAssignValues, MemAssignValues, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemAssignScores, MemAssignScores, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemInsertAndEvict, MemInsertAndEvict, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemInsertOrAssign, MemInsertOrAssign, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemAccumOrAssign, MemAccumOrAssign, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemErase, MemErase, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemEraseIf, MemEraseIf, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemClear, MemClear, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemExportBatch, MemExportBatch, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemExportBatchIf, MemExportBatchIf, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemEmpty, MemEmpty, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemSize, MemSize, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemCapacity, MemCapacity, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemReserve, MemReserve, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemSave, MemSave, NotImplemented);
PRO_DEF_WEAK_DISPATCH(WeakMemLoad, MemLoad, NotImplemented);

#define ExpandFindValueType(key_dtype, value_dtype)                       \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values)

#define ExpandFindWithMissType(key_dtype, value_dtype)                    \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<key_dtype> *missed_keys,                  \
                  Eigen::ArrayX<int> *missed_indices, int *missed_size)

#define ExpandFindWithMissScoreType(key_dtype, value_dtype, score_type)   \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<key_dtype> *missed_keys,                  \
                  Eigen::ArrayX<int> *missed_indices, int *missed_size,   \
                  Eigen::ArrayX<score_type> *scores)

#define ExpandFindWithExistType(key_dtype, value_dtype)                   \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<bool> *exists)

#define ExpandFindWithScoreType(key_dtype, value_dtype, score_type)       \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<score_type> *scores)

#define ExpandFindWithScoreExistType(key_dtype, value_dtype, score_type)  \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<bool> *exists,                            \
                  Eigen::ArrayX<score_type> *scores)

#define ExpandFindType(key_dtype, value_dtype, score_type)           \
  ExpandFindValueType(key_dtype, value_dtype),                       \
    ExpandFindWithMissType(key_dtype, value_dtype),                  \
    ExpandFindWithMissScoreType(key_dtype, value_dtype, score_type), \
    ExpandFindWithExistType(key_dtype, value_dtype),                 \
    ExpandFindWithScoreType(key_dtype, value_dtype, score_type),     \
    ExpandFindWithScoreExistType(key_dtype, value_dtype, score_type)

#define ExpandFindOrInsertValueType(key_dtype, value_dtype)               \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values)

#define ExpandFindOrInsertWithScoreType(key_dtype, value_dtype, score_type) \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys,   \
                  Eigen::ArrayXX<value_dtype> *values,                      \
                  Eigen::ArrayX<score_type> *scores)

#define ExpandFindOrInsertWithExistType(key_dtype, value_dtype)           \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<bool> *exists)

#define ExpandFindOrInsertWithScoreExistType(key_dtype, value_dtype,      \
                                             score_type)                  \
  std::error_code(const int n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayXX<value_dtype> *values,                    \
                  Eigen::ArrayX<bool> *exists,                            \
                  Eigen::ArrayX<score_type> *scores)

#define ExpandFindOrInsertType(key_dtype, value_dtype, score_type)       \
  ExpandFindOrInsertValueType(key_dtype, value_dtype),                   \
    ExpandFindOrInsertWithScoreType(key_dtype, value_dtype, score_type), \
    ExpandFindOrInsertWithExistType(key_dtype, value_dtype),             \
    ExpandFindOrInsertWithScoreExistType(key_dtype, value_dtype, score_type)

#define ExpandContainsType(key_dtype)                                        \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  Eigen::ArrayX<bool> *exists)

#define ExpandAssignType(key_dtype, value_dtype, score_type)                 \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayXX<value_dtype> *values,                 \
                  const Eigen::ArrayX<score_type> *scores)

#define ExpandAssignValuesType(key_dtype, value_dtype)                       \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayXX<value_dtype> *values)

#define ExpandAssignScoresType(key_dtype, score_type)                        \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayX<score_type> *scores)

#define ExpandInsertAndEvictType(key_dtype, value_dtype, score_type)         \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayXX<value_dtype> *values,                 \
                  const Eigen::ArrayX<score_type> *scores,                   \
                  Eigen::ArrayX<key_dtype> *evicted_keys,                    \
                  Eigen::ArrayXX<value_dtype> *evicted_values,               \
                  Eigen::ArrayX<score_type> *evicted_scores,                 \
                  size_t *evicted_size)

#define ExpandInsertOrAssignType(key_dtype, value_dtype, score_type)         \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayXX<value_dtype> *values,                 \
                  const Eigen::ArrayX<score_type> *scores)

#define ExpandAccumOrAssignType(key_dtype, value_dtype, score_type)          \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys, \
                  const Eigen::ArrayXX<value_dtype> *values,                 \
                  const Eigen::ArrayX<bool> *accum_or_assigns,               \
                  const Eigen::ArrayX<score_type> *scores)

#define ExpandEraseType(key_dtype) \
  std::error_code(const size_t n_keys, const Eigen::ArrayX<key_dtype> *keys)

#define ExpandEraseIfType(key_dtype, score_dtype) \
  std::error_code(const key_dtype pattern, const score_dtype threshold)

#define ExpandExportBatchType(key_dtype, value_dtype, score_type)      \
  std::error_code(                                                     \
    const size_t max_batch_size, const size_t cursor_offset,           \
    size_t *exported_batch_size, const Eigen::ArrayX<key_dtype> *keys, \
    Eigen::ArrayXX<value_dtype> *values, Eigen::ArrayX<score_type> *scores)

#define ExpandExportBatchIfType(key_dtype, value_dtype, score_type)    \
  std::error_code(                                                     \
    const key_dtype pattern, const score_type threshold,               \
    const size_t max_batch_size, const size_t cursor_offset,           \
    size_t *exported_batch_size, const Eigen::ArrayX<key_dtype> *keys, \
    Eigen::ArrayXX<value_dtype> *values, Eigen::ArrayX<score_type> *scores)

// clang-format off
struct StorageInterface : pro::facade_builder
    ::add_convention<WeakMemInit, 
                     std::error_code(
                      const YAML::Node *config)>
    ::add_convention<WeakMemDevice,
                     std::expected<DeviceType, std::error_code>()>
    ::add_convention<WeakMemDim,
                     std::error_code()>
    ::add_convention<WeakMemFind,
                     ExpandFindType(int64_t, int64_t, uint64_t),
                     ExpandFindType(int64_t, int32_t, uint64_t),
                     ExpandFindType(int64_t, int8_t, uint64_t),
                     ExpandFindType(int64_t, float, uint64_t),
                     ExpandFindType(int64_t, Eigen::half, uint64_t),
                     ExpandFindType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemContains,
                     ExpandContainsType(int64_t)>
    ::add_convention<WeakMemAssign,
                     ExpandAssignType(int64_t, int64_t, uint64_t),
                     ExpandAssignType(int64_t, int32_t, uint64_t),
                     ExpandAssignType(int64_t, int8_t, uint64_t),
                     ExpandAssignType(int64_t, float, uint64_t),
                     ExpandAssignType(int64_t, Eigen::half, uint64_t),
                     ExpandAssignType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemAssignValue,
                     ExpandAssignValuesType(int64_t, int64_t),
                     ExpandAssignValuesType(int64_t, int32_t),
                     ExpandAssignValuesType(int64_t, int8_t),
                     ExpandAssignValuesType(int64_t, float),
                     ExpandAssignValuesType(int64_t, Eigen::half),
                     ExpandAssignValuesType(int64_t, Eigen::bfloat16)>
    ::add_convention<WeakMemAssignScores,
                     ExpandAssignScoresType(int64_t, uint64_t)>
    ::add_convention<WeakMemFindOrInsert,
                     ExpandFindOrInsertType(int64_t, int64_t, uint64_t),
                     ExpandFindOrInsertType(int64_t, int32_t, uint64_t),
                     ExpandFindOrInsertType(int64_t, int8_t, uint64_t),
                     ExpandFindOrInsertType(int64_t, float, uint64_t),
                     ExpandFindOrInsertType(int64_t, Eigen::half, uint64_t),
                     ExpandFindOrInsertType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemInsertAndEvict,
                     ExpandInsertAndEvictType(int64_t, int64_t, uint64_t),
                     ExpandInsertAndEvictType(int64_t, int32_t, uint64_t),
                     ExpandInsertAndEvictType(int64_t, int8_t, uint64_t),
                     ExpandInsertAndEvictType(int64_t, float, uint64_t),
                     ExpandInsertAndEvictType(int64_t, Eigen::half, uint64_t),
                     ExpandInsertAndEvictType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemInsertOrAssign,
                     ExpandInsertOrAssignType(int64_t, int64_t, uint64_t),
                     ExpandInsertOrAssignType(int64_t, int32_t, uint64_t),
                     ExpandInsertOrAssignType(int64_t, int8_t, uint64_t),
                     ExpandInsertOrAssignType(int64_t, float, uint64_t),
                     ExpandInsertOrAssignType(int64_t, Eigen::half, uint64_t),
                     ExpandInsertOrAssignType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemAccumOrAssign,
                     ExpandAccumOrAssignType(int64_t, int64_t, uint64_t),
                     ExpandAccumOrAssignType(int64_t, int32_t, uint64_t),
                     ExpandAccumOrAssignType(int64_t, int8_t, uint64_t),
                     ExpandAccumOrAssignType(int64_t, float, uint64_t),
                     ExpandAccumOrAssignType(int64_t, Eigen::half, uint64_t),
                     ExpandAccumOrAssignType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemErase,
                     ExpandEraseType(int64_t)>
    ::add_convention<WeakMemEraseIf,
                     ExpandEraseIfType(int64_t, uint64_t)>
    ::add_convention<WeakMemClear,
                     std::error_code()>
    ::add_convention<WeakMemExportBatch,
                     ExpandExportBatchType(int64_t, int64_t, uint64_t),
                     ExpandExportBatchType(int64_t, int32_t, uint64_t),
                     ExpandExportBatchType(int64_t, int8_t, uint64_t),
                     ExpandExportBatchType(int64_t, float, uint64_t),
                     ExpandExportBatchType(int64_t, Eigen::half, uint64_t),
                     ExpandExportBatchType(int64_t, Eigen::bfloat16, uint64_t)>
    ::add_convention<WeakMemExportBatchIf,
                     ExpandExportBatchIfType(int64_t, int64_t, uint64_t),
                     ExpandExportBatchIfType(int64_t, int32_t, uint64_t),
                     ExpandExportBatchIfType(int64_t, int8_t, uint64_t),
                     ExpandExportBatchIfType(int64_t, float, uint64_t),
                     ExpandExportBatchIfType(int64_t, Eigen::half, uint64_t),
                     ExpandExportBatchIfType(int64_t, Eigen::bfloat16, uint64_t)>              
    ::add_convention<WeakMemEmpty,
                     std::error_code()>
    ::add_convention<WeakMemSize,
                     std::error_code()>
    ::add_convention<WeakMemCapacity,
                     std::error_code()>
    ::add_convention<WeakMemReserve,
                     std::error_code(const size_t new_capacity)>
    ::add_convention<WeakMemSave,
                     std::error_code(const YAML::Node)>
    ::add_convention<WeakMemLoad,
                     std::error_code(const YAML::Node)>
    ::build {};  // clang-format on

#undef ExpandFindType
#undef ExpandContainsType
#undef ExpandAssignType
#undef ExpandAssignValuesType
#undef ExpandAssignScoresType
#undef ExpandFindOrInsertType
#undef ExpandInsertAndEvictType
#undef ExpandInsertOrAssignType
#undef ExpandAccumOrAssignType
#undef ExpandEraseType
#undef ExpandEraseIfType
#undef ExpandExportBatchType
#undef ExpandExportBatchIfType

}  // namespace storage
}  // namespace meepo_embedding

#endif  // MEEPO_EMBEDDING_STORAGE_INTERFACE_HPP_