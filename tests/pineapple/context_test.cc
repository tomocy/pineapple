#include "src/pineapple/context.h"

#include <iostream>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(ContextParent, SuccessInWithoutParent) {
  auto flags = flags::FlagSet("parent");

  flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  flags.Parse(std::vector<std::string>{"--aaa", "123", "child", "--bbb"});

  auto ctx = pineapple::Context(flags);

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

  auto ctx = pineapple::Context(pineapple::Context::Make(std::move(parent_ctx)),
                                flags);

  EXPECT_EQ("123", ctx.Parent()->Flag("aaa").Get<std::string>());

  EXPECT_TRUE(ctx.Flag("bbb").Get<bool>());
}

TEST(ContextArgs, Success) {
  auto flags = flags::FlagSet("flags");

  auto args = std::vector<std::string>{"a", "b", "c,d", "e"};

  flags.Parse(args);

  auto ctx = pineapple::Context(flags);

  EXPECT_EQ(args, ctx.Args());
}

TEST(ContextFlag, Success) {
  auto flags = flags::FlagSet("flags");

  flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto ctx = pineapple::Context(flags);

  EXPECT_EQ("123", ctx.Flag("aaa").Get<std::string>());
}