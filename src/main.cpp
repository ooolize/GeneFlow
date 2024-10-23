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
GENE_DEFINE(Point, (double) x, (double) y); // TODO支持(double)x
// clang-format on
int main() {
  Point point;
  load_json(point, "point.json");
  return 0;
}