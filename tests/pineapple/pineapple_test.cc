#include "src/pineapple/pineapple.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Command, Success) {
  EXPECT_NO_THROW(pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {}));
}

TEST(Command, FailedDueToEmptyName) {
  EXPECT_THROW(
      pineapple::Command("", "do something",
                         [](pineapple::Command::const_action_ctx_t _) {}),
      pineapple::Exception);
}

TEST(Command, FailedDueToNoAction) {
  EXPECT_THROW(pineapple::Command("do", "do something", nullptr),
               pineapple::Exception);
}

TEST(CommandUsage, Success) {
  auto cmd = pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {});

  EXPECT_EQ("do  do something", cmd.Usage());
}

TEST(CommandOutline, Success) {
  auto cmd = pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {});

  EXPECT_EQ("do  do something", cmd.Outline());
}

TEST(CommandRun, SuccessInWithoutArgs) {
  auto called = false;
  auto cmd = pineapple::Command(
      "do", "do something",
      [&called](pineapple::Command::const_action_ctx_t _) { called = true; });

  auto flags = flags::FlagSet("do");

  flags.Parse(std::vector<std::string>{"do"});

  EXPECT_NO_THROW(cmd.Run(pineapple::Context(std::move(flags))));

  EXPECT_TRUE(called);
}

TEST(CommandRun, SuccessInWithArgs) {
  auto cmd_args = std::string("");
  auto cmd = pineapple::Command(
      "do", "do something",
      [&cmd_args](pineapple::Command::const_action_ctx_t ctx) {
        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        cmd_args = joined.str();
      });

  auto flags = flags::FlagSet("do");

  flags.Parse(std::vector<std::string>{"do", "a", "b", "c,d", "e"});

  EXPECT_NO_THROW(cmd.Run(pineapple::Context(std::move(flags))));

  EXPECT_EQ("a,b,c,d,e,", cmd_args);
}

TEST(CommandRun, FailedDueToInsufficientArgs) {
  auto cmd = pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {});

  auto flags = flags::FlagSet("do");

  flags.Parse(std::vector<std::string>{});

  EXPECT_THROW(cmd.Run(pineapple::Context(std::move(flags))),
               pineapple::Exception);
}

TEST(CommandRun, FailedDueToExceptionFromAction) {
  auto cmd = pineapple::Command(
      "do", "do something", [](pineapple::Command::const_action_ctx_t _) {
        throw pineapple::Exception("an exception from action");
      });

  auto flags = flags::FlagSet("do");

  flags.Parse(std::vector<std::string>{"do"});

  EXPECT_THROW(cmd.Run(pineapple::Context(std::move(flags))),
               pineapple::Exception);
}

TEST(App, Success) { EXPECT_NO_THROW(pineapple::App("app", "a cli app")); }

TEST(App, FailedDueToEmptyName) {
  EXPECT_THROW(pineapple::App("", "a cli app"), pineapple::Exception);
}

TEST(AppUsage, SuccessInWithoutDescription) {
  auto app = pineapple::App("app");

  EXPECT_EQ("app", app.Usage());
}

TEST(AppUsage, SuccessInWithDescription) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_EQ("app - a cli app", app.Usage());
}

TEST(AppUsage, SuccessInWithCommands) {
  auto app = pineapple::App("app", "a cli app");

  app.AddFlag(
      flags::Flag("help", flags::Bool::Make(false), "print help message"));

  app.AddCommand(
      pineapple::Command("create", "create something",
                         [](pineapple::Command::const_action_ctx_t _) {}));
  app.AddCommand(
      pineapple::Command("delete", "delete something",
                         [](pineapple::Command::const_action_ctx_t _) {}));

  EXPECT_EQ(R"###(app - a cli app
Flags:
--help bool  print help message (default: false)
Commands:
create  create something
delete  delete something)###",
            app.Usage());
}

TEST(AppAddFlag, Success) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_NO_THROW(app.AddFlag(flags::Flag("aaa", flags::String::Make(""))));
}

TEST(AppAddFlag, FailedDueToDuplicatedFlags) {
  auto app = pineapple::App("app", "a cli app");

  app.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  EXPECT_THROW(app.AddFlag(flags::Flag("aaa", flags::String::Make(""))),
               flags::Exception);
}

TEST(AppAddCommand, Success) {
  auto app = pineapple::App("app", "a cli app");

  EXPECT_NO_THROW(app.AddCommand(pineapple::Command(
      "do", "do somethiing", [](pineapple::Command::const_action_ctx_t _) {})));
}

TEST(AppAddCommand, FailedDueToDuplicatedCommands) {
  auto app = pineapple::App("app", "a cli app");

  auto cmd = pineapple::Command(
      "do", "do somethiing", [](pineapple::Command::const_action_ctx_t _) {});

  app.AddCommand(cmd);

  EXPECT_THROW(app.AddCommand(cmd), pineapple::Exception);
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

TEST(ContextParent, SuccessInWithoutParent) {
  auto flags = flags::FlagSet("parent");

  flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  flags.Parse(std::vector<std::string>{"--aaa", "123", "child", "--bbb"});

  auto ctx = pineapple::Context(std::move(flags));

  EXPECT_EQ("123", ctx.Flag("aaa").Get<std::string>());
  EXPECT_EQ(nullptr, ctx.Parent());
}

TEST(ContextParent, SuccessInWithParent) {
  auto parent_flags = flags::FlagSet("parent");

  parent_flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  parent_flags.Parse(
      std::vector<std::string>{"--aaa", "123", "child", "--bbb"});

  auto parent_ctx = pineapple::Context(std::move(parent_flags));

  auto flags = flags::FlagSet("child");

  flags.AddFlag(flags::Flag("bbb", flags::Bool::Make(false)));

  flags.Parse(std::vector<std::string>{"--bbb"});

  auto ctx = pineapple::Context(std::move(parent_ctx), std::move(flags));

  EXPECT_EQ("123", ctx.Parent()->Flag("aaa").Get<std::string>());

  EXPECT_TRUE(ctx.Flag("bbb").Get<bool>());
}

TEST(ContextArgs, Success) {
  auto flags = flags::FlagSet("flags");

  auto args = std::vector<std::string>{"a", "b", "c,d", "e"};

  flags.Parse(args);

  auto ctx = pineapple::Context(std::move(flags));

  EXPECT_EQ(args, ctx.Args());
}

TEST(ContextFlag, Success) {
  auto flags = flags::FlagSet("flags");

  flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto ctx = pineapple::Context(std::move(flags));

  EXPECT_EQ("123", ctx.Flag("aaa").Get<std::string>());
}