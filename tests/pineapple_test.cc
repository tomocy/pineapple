#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Command, Help) {
  auto app = pineapple::Command("test", "a cli");
  app.AddCommand(pineapple::Command("sub1", "a subcommand"));
  EXPECT_EQ(app.Help(), R"(test - a cli

Commands:
sub1    a subcommand)");
}