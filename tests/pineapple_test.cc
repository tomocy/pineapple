#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Pineapple, Success) {
  auto flag1 = std::string("");
  auto flag2 = std::string("");
  auto rest_args = std::string("");

  auto args = std::vector<std::string>{
      "/program", "test", "--flag1", "1", "--flag2", "2", "a", "b", "c,d,e"};

  auto app = pineapple::App();
  app.AddCommand(pineapple::Command("test").WithAction(
      [&flag1, &flag2, &rest_args](const pineapple::Context& ctx) {
        flag1 = ctx.Get("flag1");
        flag2 = ctx.Get("flag2");

        auto args = ctx.Args();
        std::ostringstream joined_rest_args;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined_rest_args, ","));
        rest_args = joined_rest_args.str();
      }));

  app.Run(args);

  EXPECT_EQ(flag1, "1");
  EXPECT_EQ(flag2, "2");
  EXPECT_EQ(rest_args, "a,b,c,d,e,");
}