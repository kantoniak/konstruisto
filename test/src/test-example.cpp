#include <string>

#include <gtest/gtest.h>

TEST(ExampleTest, Static) {
  EXPECT_EQ(std::string("Example"), std::string("Example"));
}