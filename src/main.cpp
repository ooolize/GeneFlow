/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-23
 * @LastEditors: lize
 */
#include <fmt/core.h>

#include <iostream>
#include <string>

#include "interface/define.h"
#include "load.h"
#include "print_field.h"
constexpr std::string file_path = "point.json";

// clang-format off
GENE_DEFINE(Color, (int) r,(int) g,(int) b); 
GENE_DEFINE(Point, (double) x, (double) y,(double) z,(Color) color); // TODO支持(double)x
GENE_DEFINE(Config, (Point) point,(std::string) description);
// clang-format on

// struct Point {
//   static constexpr std::size_t fields_count = 2;
//   template <typename T, std::size_t N>
//   struct Field;
//   double x;
//   template <typename T>
//   struct Field<T, 0> {
//     T& obj;
//     decltype(auto) value() { // 可以通过这个获取属性的类型
//       return (obj.x); c++魅力时刻之需要返回的是左值引用
//     }
//     static constexpr const char* name() {
//       return "x";
//     }
//   };
//   double y;
//   template <typename T>
//   struct Field<T, 0 + 1> {
//     T& obj;
//     decltype(auto) value() {
//       return obj.y;
//     }
//     static constexpr const char* name() {
//       return "y";
//     }
//   };
// };
int main() {
  Config config{};
  auto ret = lz::GeneFlow::load_json(config, "point.json");
  if (ret != lz::GeneFlow::Result::SUCCESS) {
    std::cout << "load json failed " << static_cast<std::int32_t>(ret)
              << std::endl;
    return -1;
  }
  // fmt::print("x: {}, y: {}\n", config.point.x, config.point.y);
  lz::GeneFlow::print_config(config);
  return 0;
}