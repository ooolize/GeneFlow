/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-23
 * @LastEditors: lize
 */
#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace lz {
namespace GeneFlow {

// 解析库的包装  需要重建json符合定义的接口形式
class JsonParse {
 public:
  struct JsonNode;
  using Elem = JsonNode;
  using NodeUptr = std::unique_ptr<JsonNode>;
  using NodeSptr = std::shared_ptr<JsonNode>;
  explicit JsonParse(std::string_view content) {
    _content = content;
  }
  Result parse() {
    try {
      _json = nlohmann::json::parse(_content);
    } catch (nlohmann::json::parse_error& e) {
      // 捕获解析错误并输出错误信息
      std::cerr << "JSON 解析错误: " << e.what() << std::endl;
      std::cerr << "错误发生在第 " << e.byte << " 个字节" << std::endl;
      return Result::FORMAT_ERROR;
    }
    // 构建树
    dfs(_json, _root.get());
    return Result::SUCCESS;
  };
  struct JsonNode {
    JsonNode() = default;
    explicit JsonNode(std::string_view key, std::string_view value)
      : _key(key), _value_text(value) {
    }
    decltype(auto) get_value_text() const {
      return std::string(_value_text);
    }
    JsonNode get_child_elem(std::string_view name) const {
      if (_children.find(name.data()) == _children.end()) {
        return JsonNode{};
      }
      return *_children.at(name.data());
    }
    std::string_view _key{};
    std::string_view _value_text{};
    std::map<std::string_view, NodeSptr> _children{};
  };

  JsonNode getRootElem() const {
    return *_root;
  }

 private:
  // {
  //   "a":{
  //    "1":2 node
  //   }
  // }

  void dfs(const nlohmann::json& j, JsonNode* node) {
    if (j.is_object()) {
      for (auto& [key, value] : j.items()) {
        auto child = std::make_shared<JsonNode>(key, value.dump());
        node->_children[key] = child;
        dfs(value, child.get());
      }
    }
  }

 private:
  std::string_view _content{};
  nlohmann::json _json{};
  std::unique_ptr<JsonNode> _root{};
};
}  // namespace GeneFlow
}  // namespace lz