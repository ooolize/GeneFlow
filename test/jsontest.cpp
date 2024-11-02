/*
 * @Description:
 * @Author: lize
 * @Date: 2024-09-23
 * @LastEditors: lize
 */

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "interface/define.h"
#include "load.h"

namespace SimpleJsonTest {
// clang-format off
GENE_DEFINE(Point, (double) x, (double) y, (double) z);
GENE_DEFINE(Config, (Point) point);
// clang-format on
}  // namespace SimpleJsonTest

namespace NestJsonTest {
// clang-format off
GENE_DEFINE(Color, (int) r,(int) g,(int) b); 
GENE_DEFINE(Point, (double) x, (double) y,(double) z,(Color) color); 
GENE_DEFINE(Config, (Point) point,(std::string) description);
// clang-format on
}  // namespace NestJsonTest

namespace lz {
namespace test {
class JsonTest : public testing::Test {  // NOLINT
 protected:
  JsonTest() {
  }
  ~JsonTest() override {
  }
  void SetUp() override {
  }
  void TearDown() override {
  }
};
TEST_F(JsonTest, SimpleTest) {
  SimpleJsonTest::Config config{};
  auto ret = lz::GeneFlow::load_json(config, "data/json/simple.json");
  ASSERT_EQ(ret, lz::GeneFlow::Result::SUCCESS);
}

TEST_F(JsonTest, NestJsonTest) {
  NestJsonTest::Config config{};
  auto ret = lz::GeneFlow::load_json(config, "data/json/nest.json");
  ASSERT_EQ(ret, lz::GeneFlow::Result::SUCCESS);
}

TEST_F(JsonTest, TypeErrorTest) {
  NestJsonTest::Config config{};
  auto ret = lz::GeneFlow::load_json(config, "data/json/type_error.json");
  ASSERT_EQ(ret, lz::GeneFlow::Result::NOT_NUMBER);
}

}  // namespace test
}  // namespace lz