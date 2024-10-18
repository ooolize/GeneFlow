/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-23
 * @LastEditors: lize
 */

#include <fmt/core.h>
#include <gtest/gtest.h>

namespace lz {
namespace test {
class PlaceholderTest : public testing::Test {};
TEST_F(PlaceholderTest, PlaceholderTest1) {
  EXPECT_EQ(1, 1);
}
}  // namespace test
}  // namespace lz