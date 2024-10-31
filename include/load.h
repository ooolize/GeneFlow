/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-21
 * @LastEditors: lize
 */
#pragma once
#include "detail.h"
#include "interface/define.h"
#include "json_parse.h"
#include "use_concept.h"
namespace lz {
namespace GeneFlow {

template <typename T>
struct DummyStruct {
  static constexpr std::size_t fields_count = 1;
  template <typename U, std::size_t N>
  struct Field;
  T& data;

  template <typename U>
  struct Field<U, 0> {
    U& obj;
    decltype(auto) value() {
      return obj.data;
    }
    static constexpr const char* name() {
      return "point";
    }
  };
};

// 负责载入输入，并开始解析
template <lz::use_concepts::Parse Parser>
class LoadObj {
 public:
  template <typename Obj, std::invocable GET_CONTENT>
  Result operator()(Obj& obj, GET_CONTENT content_getter) {
    std::string data = content_getter();
    Parser p(data);
    auto ret = p.parse();
    if (ret != Result::SUCCESS) {
      return ret;
    }
    DummyStruct<Obj> dummy{obj};
    decltype(auto) root = p.getRootElem();
    // dump<Obj>{};
    ret = lz::GeneFlow::checkElem(dummy, std::forward<decltype(root)>(root));
    return ret;
  };

  template <typename Obj>
  Result operator()(Obj& obj, const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
      return Result::FILE_OPEN_FAILED;
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    auto sp_content = std::make_shared<std::string>(std::move(content));
    return (*this)(obj, [sp_content]() { return *sp_content; });
  }
};

template <typename T, typename Content>
decltype(auto) load_json(T& t, Content&& content) {
  // dump<Content>{};
  return LoadObj<JsonParse>{}(t, std::forward<Content>(content));
}

}  // namespace GeneFlow
}  // namespace lz