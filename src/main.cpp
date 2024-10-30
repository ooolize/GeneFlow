/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-23
 * @LastEditors: lize
 */
#include <fmt/core.h>

#include <string>

#include "interface/define.h"
#include "load.h"

constexpr std::string file_path = "point.json";

// clang-format off
// GENE_DEFINE(Point, (double) x, (double) y); // TODO支持(double)x
// clang-format on

struct Point {
  static constexpr std::size_t fields_count = 2;
  template <typename T, std::size_t N>
  struct Field;
  double x;
  template <typename T>
  struct Field<T, 0> {
    T& obj;
    decltype(auto) value() {
      return obj.x;
    }
    static constexpr const char* name() {
      return "x";
    }
  };
  double y;
  template <typename T>
  struct Field<T, 0 + 1> {
    T& obj;
    decltype(auto) value() {
      return obj.y;
    }
    static constexpr const char* name() {
      return "y";
    }
  };
};
int main() {
  Point point{};
  lz::GeneFlow::load_json(point, "point.json");
  return 0;
}