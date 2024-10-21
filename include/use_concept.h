/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-21
 * @LastEditors: lize
 */

#pragma once

#include <concepts>
#include <type_traits>
namespace lz {
namespace use_concepts {

template <typename T>
concept Reflect = requires(T t) {
  { T::fields_count } -> std::convertible_to<std::size_t>;
  requires(T::fields_count == 0) || (T::fields_count > 0 && requires {
            std::decay_t<T>::template Field<T, 0>;
          });
};
//  使用std::decay_t<T>而不是T，首先传入的是T可能带有const/&/volatile等修饰符，
//  我们无法保证带有这个修饰符的类型 事先有特化定义。
//  所以我们使用std::decay_t<T>来去除这些修饰符。

template <typename T>
concept Elem = requires(T t) {
  t.get_name();
  t.get_value_text();
  t.get_child_elem();
};

template <typename T>
concept Parse = requires(T t) {
  typename T::Elem;
  t.parse();
  { t.getRootElem() } -> std::same_as<typename T::Elem>;
};
}  // namespace use_concepts
}  // namespace lz