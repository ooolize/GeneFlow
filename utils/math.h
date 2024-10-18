/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-29
 * @LastEditors: lize
 */

#include <algorithm>
#include <iostream>
#include <numeric>

#include "use_concept.h"

namespace lz {
namespace math {

template <lz::use_concept::RandomAccessSequence Container>
void calculate_statistics(const Container& data) {
  using T = typename Container::value_type;
  if (data.empty()) {
    std::cout << "The Container is empty." << std::endl;
    return;
  }

  // 计算最小值和最大值
  T min_value = *std::min_element(data.begin(), data.end());
  T max_value = *std::max_element(data.begin(), data.end());

  // 计算平均值
  T sum = std::accumulate(data.begin(), data.end(), 0.0);
  T mean = sum / data.size();

  // 计算中位数
  Container sorted_data = data;  // 复制原始数据
  std::sort(sorted_data.begin(), sorted_data.end());
  T median;
  if (sorted_data.size() % 2 == 0) {
    // 偶数个元素，取中间两个的平均值
    median = (sorted_data[sorted_data.size() / 2 - 1] +
              sorted_data[sorted_data.size() / 2]) /
             2.0;
  } else {
    // 奇数个元素，取中间值
    median = sorted_data[sorted_data.size() / 2];
  }

  // 计算第95百分位数
  T percentile_95;
  size_t index_95 = static_cast<size_t>(std::ceil(0.95 * sorted_data.size())) -
                    1;  // 索引从0开始，所以需要减1
  percentile_95 = sorted_data[index_95];

  // 输出结果
  std::cout << "Min: " << min_value << std::endl;
  std::cout << "Max: " << max_value << std::endl;
  std::cout << "Mean: " << mean << std::endl;
  std::cout << "Median: " << median << std::endl;
  std::cout << "95th Percentile: " << percentile_95 << std::endl;
}

}  // namespace math
}  // namespace lz
