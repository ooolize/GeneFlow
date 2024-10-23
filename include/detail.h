/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-23
 * @LastEditors: lize
 */
#pragma once
#include <istream>

#include "interface/define.h"
#include "use_concept.h"

namespace lz {
namespace GeneFlow {

template <lz::use_concepts::Number Number, lz::use_concepts::Elem Elem>
Result checkBaseElem(Number& number, Elem auto&& elem) {
  // TODO hex
  std::istringstream is(elem.get_value_text());
  if (is.fail()) {
    return Result::NOT_NUMBER;
  }
  is >> number;
  return Result::SUCCESS;
}

template <lz::use_concepts::Bool Bool, lz::use_concepts::Elem Elem>
Result checkBaseElem(Bool& b, Elem auto&& elem) {
  // elem不应该是 true 而不是 "true"吗
  auto text = elem.get_value_text();
  if (text == "True" || text == "true") {
    b = true;
  } else if (text == "False" || text == "false") {
    b = false;
  } else {
    return Result::NOT_BOOL;
  }
  return Result::SUCCESS;
}

template <lz::use_concepts::String String, lz::use_concepts::Elem Elem>
Result checkBaseElem(String& s, Elem auto&& elem) {
  auto value = elem.get_value_text();
  if constexpr (!std::same_as<String, std::string>) {
    return Result::NOT_STRING;
  }
  s = value;
  return Result::SUCCESS;
}

template <lz::use_concepts::BasicType BasicType, lz::use_concepts::Elem Elem>
Result checkElem(BasicType& t, Elem auto&& elem) {
  return checkBaseElem(t, std::forward(elem));
}

template <lz::use_concepts::Reflect Reflect, lz::use_concepts::Elem Elem>
Result checkElem(Reflect& obj, Elem auto&& elem) {
  auto f = [&](auto& field) {
    decltype(auto) name = field.name();
    decltype(auto) value = field.value();
    return checkElem(name, elem.get_child_elem(name));
  };
  return for_each_field(obj, f);
}

}  // namespace GeneFlow
}  // namespace lz