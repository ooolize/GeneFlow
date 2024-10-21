/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-23
 * @LastEditors: lize
 */
#include <fmt/core.h>

#include <string>

constexpr std::string file_path = "point.json";

int main() {
  GENEDEFINE(Point, (double)x, (double)y);
  Point point;
  LoadObj(point, "point.json");
  return 0;
}