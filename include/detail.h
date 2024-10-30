/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-23
 * @LastEditors: lize
 */
#pragma once
#include <iostream>
#include <sstream>

#include "check_field.h"
#include "interface/define.h"
#include "use_concept.h"

namespace lz {
namespace GeneFlow {

template <lz::use_concepts::Number Number, lz::use_concepts::Elem Elem>
Result checkBaseElem(Number& number, Elem&& elem) {
  // TODO hex
  std::istringstream is(elem.get_value_text());
  if (is.fail()) {
    return Result::NOT_NUMBER;
  }
  is >> number;
  return Result::SUCCESS;
}

template <lz::use_concepts::Bool Bool, lz::use_concepts::Elem Elem>
Result checkBaseElem(Bool& b, Elem&& elem) {
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
Result checkBaseElem(String& s, Elem&& elem) {
  // lz::use_concepts::dump<decltype(s)>{};
  auto value = elem.get_value_text();
  if constexpr (!std::same_as<String, std::string>) {
    return Result::NOT_STRING;
  }
  s = value.data();
  return Result::SUCCESS;
}

template <lz::use_concepts::BasicType BasicType>
Result checkElem(BasicType& t, lz::use_concepts::Elem auto&& elem) {
  return checkBaseElem(t, std::forward<decltype(elem)>(elem));
}

// obj是最后的结果 elem是预先建立的树的节点
template <lz::use_concepts::Reflect Reflect>
Result checkElem(Reflect& obj, lz::use_concepts::Elem auto&& elem) {
  auto f = [elem](auto&& field) {
    decltype(auto) name = field.name();
    // decltype(auto) value = field.value();
    return checkElem(name, elem.get_child_elem(name));  // why not  const
  };
  return for_each_field(obj, std::forward<decltype(f)>(f));
}

}  // namespace GeneFlow
}  // namespace lz