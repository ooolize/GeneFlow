/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-30
 * @LastEditors: lize
 */
#pragma once
#include <utility>

#include "interface/define.h"
#include "use_concept.h"
namespace lz {
namespace GeneFlow {

struct DummyField {
  const char* name() const;
  int value() const;
};

// 对每个field 调用f(field)
// 需要整数类型(非类型形参)的参数包size_t...Index 而不是 std::index_sequence
template <lz::use_concepts::Reflect Reflect, typename F, std::size_t... Index>
static constexpr auto for_each_field_impl(Reflect& obj,
                                          F&& f,
                                          std::index_sequence<Index...>) {
  //  using f_res = decltype(std::declval<F>()(std::declval<DummyField>()));
  using f_res = decltype(f(std::declval<DummyField>()));

  if constexpr (std::is_same_v<void, f_res>) {
    (f(typename Reflect::template Field<Reflect, Index>{obj}), ...);
  } else {
    Result res = Result::SUCCESS;
    // （(f(Index)|| ...)) 外部括号是必须的
    // 聚合初始化 field绑定实际对象的引用， 在f中field被分解或赋值
    // 当所有field都被赋值后，它绑定的实际对象也就获取了它该有的值
    if ((((res = f(typename Reflect::template Field<Reflect, Index>{obj})) !=
          Result::SUCCESS) ||
         ...)) {
      return res;
    }
    return res;
  }
}

// 负责对reflect的节点元素的每个field进行判断
// TODO why std::invockable can work
template <lz::use_concepts::Reflect Reflect, typename F>
static constexpr auto for_each_field(Reflect& obj, F&& f) {
  return for_each_field_impl(
    obj, std::forward<F>(f), std::make_index_sequence<Reflect::fields_count>{});
}
}  // namespace GeneFlow
}  // namespace lz