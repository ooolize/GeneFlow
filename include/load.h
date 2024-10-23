/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-21
 * @LastEditors: lize
 */
#pragma once
#include "interface/define.h"
#include "json_parse.h"
#include "use_concept.h"
namespace lz {
namespace GeneFlow {

// 负责载入输入，并开始解析
template <lz::use_concepts::Parse T>
class LoadObj {
 public:
  template <typename Obj, std::invocable GET_CONTENT>
  Result operator()(Obj& obj, GET_CONTENT content) {
    auto content = content();
    Parse p(content);
    auto ret = p.parse();
    if (ret != Result::SUCCESS) {
      return ret;
    }

    if (!p.checkFormat()) {
      return Result::FAILED;
    }

    decltype(auto) root = p.getRootElem();
    return checkElem(obj, std::forward<decltype(root)>(root));
  };

  Result operator()(Obj& obj, std::string_view file_path) {
    std::ifstream ifs(file_path);
    if (!ifs.is_open()) {
      return Result::FILE_OPEN_FAILED;
    }
    return (*this)(obj, [ifs, content = std::string_view{}]() {
      ifs >> content;
      return content;
    });
  }
};

template <typename T, typename Content>
decltype(auto) load_json(T& t, Content&& content) {
  return LoadObj<JsonParse>(t, std::forward<Content>(content));
}

// 负责对reflect的节点元素的每个field进行判断
template <lz::use_concepts::Reflect Reflect, std::invocable F>
static constexpr Result for_each_field(Reflect& obj, F& f) {
  for (std::size_t i = 0; i < Reflect::fields_count; ++i) {
    auto& field = obj.template Field<Reflect, i>;
    if (auto res = f(field); res != Result::SUCCESS) {
      return res;
    }
  }
  return Result::SUCCESS;
}

}  // namespace GeneFlow
}  // namespace lz