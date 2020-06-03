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

TEST(AppRun, Success) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{"/app"}));
}

TEST(AppRun, SuccessInCommandWithoutArgs) {
  auto app = pineapple::App("app", "a cli app");

  auto called = false;
  app.AddCommand(pineapple::Command("do", "do something",
                                    [&called](auto _) { called = true; }));

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{"/app", "do"}));

  EXPECT_TRUE(called);
}

TEST(AppRun, SuccessInCommandWithArgs) {
  auto app = pineapple::App("app", "a cli app");

  auto do_args = std::string("");
  app.AddCommand(
      pineapple::Command("do", "do something", [&do_args](auto args) {
        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        do_args = joined.str();
      }));

  EXPECT_NO_THROW(
      app.Run(std::vector<std::string>{"/app", "do", "a", "b", "c,d", "e"}));

  EXPECT_EQ("a,b,c,d,e,", do_args);
}

TEST(AppRun, FailedDueToInsufficientArgs) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_THROW(app.Run(std::vector<std::string>()), pineapple::Exception);
}

TEST(AppRun, FailedDueToUnknownCommand) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_THROW(app.Run(std::vector<std::string>{"/app", "do"}),
               pineapple::Exception);
}

TEST(AppRun, FailedDueToExceptionFromCommand) {
  auto app = pineapple::App("app", "a cli app");

  app.AddCommand(pineapple::Command("do", "do something", [](auto _) {
    throw pineapple::Exception("something wrong");
  }));

  EXPECT_THROW(app.Run(std::vector<std::string>{"/app", "do"}),
               pineapple::Exception);
}