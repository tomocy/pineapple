#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/pineapple/pineapple.h"

TEST(App, Success) { EXPECT_NO_THROW(pineapple::App("app", "a cli app")); }

TEST(App, FailedDueToEmptyName) {
  EXPECT_THROW(pineapple::App("", "a cli app"), pineapple::Exception);
}

TEST(AppRun, Success) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{"/app"}));
}

TEST(AppRun, SuccessInActionWithoutArgs) {
  auto called = false;
  auto app = pineapple::App(
      "app", "a cli app",
      [&called](pineapple::App::const_action_ctx_t _) { called = true; });

  EXPECT_NO_THROW(
      app.Run(std::vector<std::string>{"/app", "a", "b", "c,d", "e"}));

  EXPECT_TRUE(called);
}

TEST(AppRun, SuccessInActionWithFlagAndArgs) {
  auto app_aaa_flag = std::string("");
  auto app_bbb_flag = false;
  auto app_args = std::string("");
  auto app = pineapple::App(
      "app", "a cli app",
      [&app_aaa_flag, &app_bbb_flag,
       &app_args](pineapple::App::const_action_ctx_t ctx) {
        app_aaa_flag = ctx.Flag("aaa").Get<std::string>();
        app_bbb_flag = ctx.Flag("bbb").Get<bool>();

        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        app_args = joined.str();
      });

  app.AddFlag(flags::Flag("aaa", flags::String::Make("")));
  app.AddFlag(flags::Flag("bbb", flags::Bool::Make(false)));

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{
      "/app", "--aaa", "123", "--bbb", "a", "b", "c,d", "e"}));

  EXPECT_EQ("123", app_aaa_flag);

  EXPECT_TRUE(app_bbb_flag);

  EXPECT_EQ("a,b,c,d,e,", app_args);
}

TEST(AppRun, SuccessInCommandWithoutArgs) {
  auto app = pineapple::App("app", "a cli app");

  auto called = false;
  app.AddCommand(pineapple::Command(
      "do", "do something",
      [&called](pineapple::Command::const_action_ctx_t _) { called = true; }));

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{"/app", "do"}));

  EXPECT_TRUE(called);
}

TEST(AppRun, SuccessInCommandWithArgs) {
  auto app = pineapple::App("app", "a cli app");

  auto do_args = std::string("");
  app.AddCommand(pineapple::Command(
      "do", "do something",
      [&do_args](pineapple::Command::const_action_ctx_t ctx) {
        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        do_args = joined.str();
      }));

  EXPECT_NO_THROW(
      app.Run(std::vector<std::string>{"/app", "do", "a", "b", "c,d", "e"}));

  EXPECT_EQ("a,b,c,d,e,", do_args);
}

TEST(AppRun, SuccessInCommandWithParentArgs) {
  auto app = pineapple::App("app", "a cli app");

  app.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  auto parent_aaa_flag = std::string("");
  auto do_args = std::string("");
  app.AddCommand(pineapple::Command(
      "do", "do something",
      [&parent_aaa_flag, &do_args](pineapple::Command::const_action_ctx_t ctx) {
        parent_aaa_flag = ctx.Parent()->Flag("aaa").Get<std::string>();

        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        do_args = joined.str();
      }));

  EXPECT_NO_THROW(app.Run(std::vector<std::string>{"/app", "--aaa", "123", "do",
                                                   "a", "b", "c,d", "e"}));

  EXPECT_EQ("123", parent_aaa_flag);

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

  app.AddCommand(pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {
        throw pineapple::Exception("something wrong");
      }));

  EXPECT_THROW(app.Run(std::vector<std::string>{"/app", "do"}),
               pineapple::Exception);
}
