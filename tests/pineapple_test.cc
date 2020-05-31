#include "src/pineapple.h"

#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Pineapple, Command) {
  auto s = std::string("");

  auto args = std::vector<std::string>{"/program", "test"};

  auto app = pineapple::App();
  app.AddCommand(pineapple::Command("test").WithAction(
      [&s](const pineapple::Context& ctx) { s = "called"; }));
  app.Run(args);

  EXPECT_EQ(s, "called");
}