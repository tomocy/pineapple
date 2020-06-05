#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/pineapple/pineapple.h"

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

TEST(CommandAddFlag, Success) {
  auto cmd = pineapple::Command(
      "app", "a cli app", [](pineapple::Command::const_action_ctx_t _) {});

  EXPECT_NO_THROW(cmd.AddFlag(flags::Flag("aaa", flags::String::Make(""))));
}

TEST(CommandAddFlag, FailedDueToDuplicatedFlags) {
  auto cmd = pineapple::Command(
      "app", "a cli app", [](pineapple::Command::const_action_ctx_t _) {});

  cmd.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  EXPECT_THROW(cmd.AddFlag(flags::Flag("aaa", flags::String::Make(""))),
               flags::Exception);
}

TEST(CommandAddCommand, Success) {
  auto cmd = pineapple::Command(
      "cmd", "a command", [](pineapple::Command::const_action_ctx_t _) {});

  EXPECT_NO_THROW(cmd.AddCommand(pineapple::Command(
      "do", "do somethiing", [](pineapple::Command::const_action_ctx_t _) {})));
}

TEST(CommandAddCommand, FailedDueToDuplicatedCommands) {
  auto cmd = pineapple::Command(
      "cmd", "a command", [](pineapple::Command::const_action_ctx_t _) {});

  cmd.AddCommand(pineapple::Command(
      "do", "do somethiing", [](pineapple::Command::const_action_ctx_t _) {}));

  EXPECT_THROW(cmd.AddCommand(pineapple::Command(
                   "do", "do somethiing",
                   [](pineapple::Command::const_action_ctx_t _) {})),
               pineapple::Exception);
}

TEST(CommandRun, SuccessInWithFlagsAndArgs) {
  auto cmd_aaa_flag = std::string("");
  auto cmd_args = std::string("");
  auto cmd = pineapple::Command(
      "do", "do something",
      [&cmd_aaa_flag, &cmd_args](pineapple::Command::const_action_ctx_t ctx) {
        cmd_aaa_flag = ctx.Flag("aaa").Get<std::string>();

        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        cmd_args = joined.str();
      });

  cmd.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  auto flags = flags::FlagSet("do");

  flags.Parse(
      std::vector<std::string>{"do", "--aaa", "123", "a", "b", "c,d", "e"});

  EXPECT_NO_THROW(cmd.Run(pineapple::Context(std::move(flags))));

  EXPECT_EQ("123", cmd_aaa_flag);

  EXPECT_EQ("a,b,c,d,e,", cmd_args);
}

TEST(CommandRun, SuccessInCommandWithoutArgs) {
  auto cmd = pineapple::App("cmd", "a command");

  auto called = false;
  cmd.AddCommand(pineapple::Command(
      "do", "do something",
      [&called](pineapple::Command::const_action_ctx_t _) { called = true; }));

  EXPECT_NO_THROW(cmd.Run(std::vector<std::string>{"/app", "do"}));

  EXPECT_TRUE(called);
}

TEST(CommandRun, SuccessInCommandWithArgs) {
  auto cmd = pineapple::App("cmd", "a command");

  auto do_args = std::string("");
  cmd.AddCommand(pineapple::Command(
      "do", "do something",
      [&do_args](pineapple::Command::const_action_ctx_t ctx) {
        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        do_args = joined.str();
      }));

  EXPECT_NO_THROW(
      cmd.Run(std::vector<std::string>{"/app", "do", "a", "b", "c,d", "e"}));

  EXPECT_EQ("a,b,c,d,e,", do_args);
}

TEST(CommandRun, SuccessInCommandWithParentFlags) {
  auto cmd = pineapple::App("cmd", "a command");

  cmd.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  auto parent_aaa_flag = std::string("");
  auto do_args = std::string("");
  cmd.AddCommand(pineapple::Command(
      "do", "do something",
      [&parent_aaa_flag, &do_args](pineapple::Command::const_action_ctx_t ctx) {
        parent_aaa_flag = ctx.Parent()->Flag("aaa").Get<std::string>();

        auto args = ctx.Args();

        std::ostringstream joined;
        std::copy(std::begin(args), std::end(args),
                  std::ostream_iterator<std::string>(joined, ","));

        do_args = joined.str();
      }));

  EXPECT_NO_THROW(cmd.Run(std::vector<std::string>{
      "./cmd", "--aaa", "123", "do", "a", "b", "c,d", "e"}));

  EXPECT_EQ("123", parent_aaa_flag);

  EXPECT_EQ("a,b,c,d,e,", do_args);
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
