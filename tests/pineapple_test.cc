#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Command, Success) {
  EXPECT_NO_THROW(pineapple::Command("do", "do something", [](auto args) {}));
}

TEST(Command, FailedDueToEmptyName) {
  EXPECT_THROW(pineapple::Command("", "do something", [](auto args) {}),
               pineapple::Exception);
}

TEST(Command, FailedDueToNoAction) {
  EXPECT_THROW(pineapple::Command("do", "do something", nullptr),
               pineapple::Exception);
}

TEST(CommandUsage, Success) {
  auto cmd = pineapple::Command("do", "do something", [](auto args) {});

  EXPECT_EQ("do  do something", cmd.Usage());
}

TEST(App, Success) { EXPECT_NO_THROW(pineapple::App("app", "a cli app")); }

TEST(App, FailedDueToEmptyName) {
  EXPECT_THROW(pineapple::App("", "a cli app"), pineapple::Exception);
}

TEST(AppAddCommand, Success) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_NO_THROW(app.AddCommand(
      pineapple::Command("do", "do somethiing", [](auto args) {})));
}

TEST(AppAddCommand, FailedDueToDuplicatedCommands) {
  auto app = pineapple::App("app", "a cli app");

  auto cmd = pineapple::Command("do", "do somethiing", [](auto args) {});

  app.AddCommand(cmd);

  EXPECT_THROW(app.AddCommand(cmd), pineapple::Exception);
}