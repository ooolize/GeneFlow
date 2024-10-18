/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-29
 * @LastEditors: lize
 */
#include <concepts>
#include <ranges>

namespace lz {
namespace use_concept {
template <typename T>
concept RandomAccessSequence =
  std::ranges::range<T> &&   // 容器必须是一个范围（range）
  requires(T t) {            // 容器必须支持以下操作
    typename T::value_type;  // 有 value_type 成员
    typename T::size_type;   // 有 size_type 成员
    typename T::iterator;    // 有 iterator 成员
    { t.begin() } -> std::random_access_iterator;  // 支持随机访问迭代器
    { t.end() } -> std::random_access_iterator;  // 支持随机访问迭代器
  };
}  // namespace use_concept
}  // namespace lz
