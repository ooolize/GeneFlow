/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-23
 * @LastEditors: lize
 */
#pragma once
#include <fmt/core.h>

#include <iostream>
#include <sstream>

#include "check_field.h"
#include "interface/define.h"
#include "use_concept.h"

namespace lz {
namespace GeneFlow {

// template <lz::use_concepts::Number Number>
// Result print_config(Number& number) {
// }

// template <lz::use_concepts::Bool Bool>
// Result print_config(Bool& b) {
// }

// template <lz::use_concepts::String String>
// Result print_config(String& s) {
// }

template <lz::use_concepts::BasicType BasicType>
void print_config(BasicType& t, const std::string& sep = "") {
  //   print_config(t, std::forward<decltype(elem)>(elem));
  fmt::print("{}\n", t);
}

template <lz::use_concepts::Reflect Reflect>
void print_config(Reflect& obj, const std::string& sep = "") {
  auto f = [sep](auto&& field) {
    decltype(auto) name = field.name();
    decltype(auto) value = field.value();
    fmt::print("{}{}: ", sep, name);
    if constexpr (!lz::use_concepts::BasicType<
                    std::remove_cvref_t<decltype(value)>>) {
      fmt::print("\n");
    }
    print_config(value, sep + "\t");
    return Result::SUCCESS;
  };
  for_each_field(obj, std::forward<decltype(f)>(f));
}
}  // namespace GeneFlow
}  // namespace lz