#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Command, Help) {
  auto app = pineapple::Command("test", "a cli");
  EXPECT_EQ(app.Help(), R"(test - a cli)");
}