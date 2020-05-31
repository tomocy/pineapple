#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(FlagSet, Parse) {
  auto flags = pineapple::FlagSet(std::vector<pineapple::Flag>{
      pineapple::StringFlag("string1", ""),
  });
}

TEST(Command, Run) {
  auto s = std::string("");

  auto app = pineapple::Command("test", "a cli", [&s](auto args) {
    std::ostringstream joined;
    std::copy(std::begin(args), std::end(args),
              std::ostream_iterator<std::string>(joined, ","));
    s = joined.str();
  });

  app.Run(std::vector<std::string>{"/program", "a", "b", "c,d", "e"});

  EXPECT_EQ(s, "a,b,c,d,e,");

  app.AddCommand(pineapple::Command("sub1", "a subcommand"));

  app.Run(std::vector<std::string>{"/program", "sub1", "a", "b", "c,d", "e"});

  EXPECT_EQ(s, "a,b,c,d,e,");
}

TEST(Command, Help) {
  auto app = pineapple::Command("test", "a cli");
  app.AddCommand(pineapple::Command("sub1", "a subcommand"));
  EXPECT_EQ(app.Help(), R"(test - a cli

Commands:
sub1    a subcommand)");
}