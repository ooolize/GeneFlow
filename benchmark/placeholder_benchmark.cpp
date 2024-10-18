/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-24
 * @LastEditors: lize
 */

#include <fmt/core.h>

#include "benchmark/benchmark.h"
namespace lz {
namespace bc {
static void placeholder(benchmark::State& state) {
  for (auto _ : state) {
    fmt::print("benchmark placeholder\n");
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(placeholder)
  ->RangeMultiplier(2)
  ->Range(1 << 10, 1 << 20)
  ->Complexity();
BENCHMARK_MAIN();

};  // namespace bc

};  // namespace lz
