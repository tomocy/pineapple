#include <iostream>
#include <tuple>

#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/pineapple/pineapple.h"

TEST(ContextGlobalFlag, Success) {
  auto parent_flags = flags::FlagSet("parent");

  parent_flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  parent_flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto parent_ctx = pineapple::Context(parent_flags);

  auto flags = flags::FlagSet("child");

  flags.Parse(std::vector<std::string>());

  auto ctx = pineapple::Context(pineapple::Context::Make(std::move(parent_ctx)),
                                flags);

  EXPECT_EQ("123", ctx.GlobalFlag("aaa").Get<std::string>());
}

TEST(ContextGlobalFlag, FailedDueToUnknownFlag) {
  auto parent_flags = flags::FlagSet("parent");

  parent_flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  parent_flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto parent_ctx = pineapple::Context(parent_flags);

  auto flags = flags::FlagSet("child");

  flags.Parse(std::vector<std::string>());

  auto ctx = pineapple::Context(pineapple::Context::Make(std::move(parent_ctx)),
                                flags);

  EXPECT_THROW(ctx.GlobalFlag("bbb"), pineapple::Exception);
}

TEST(ContextTryToGetGlobalFlag, Success) {
  auto parent_flags = flags::FlagSet("parent");

  parent_flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  parent_flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto parent_ctx = pineapple::Context(parent_flags);

  auto flags = flags::FlagSet("child");

  flags.Parse(std::vector<std::string>());

  auto ctx = pineapple::Context(pineapple::Context::Make(std::move(parent_ctx)),
                                flags);

  auto [flag, ok] = ctx.TryToGetGlobalFlag("aaa");

  EXPECT_TRUE(ok);
  EXPECT_EQ("123", flag.Get<std::string>());
}

TEST(ContextTryToGlobalFlag, SuccessInUnknownFlag) {
  auto parent_flags = flags::FlagSet("parent");

  parent_flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  parent_flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto parent_ctx = pineapple::Context(parent_flags);

  auto flags = flags::FlagSet("child");

  flags.Parse(std::vector<std::string>());

  auto ctx = pineapple::Context(pineapple::Context::Make(std::move(parent_ctx)),
                                flags);

  auto [_, ok] = ctx.TryToGetGlobalFlag("bbb");

  EXPECT_FALSE(ok);
}

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

  auto parent_ctx = pineapple::Context(parent_flags);

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

TEST(ContextFlag, FailedDueToUnknownFlag) {
  auto flags = flags::FlagSet("flags");

  flags.AddFlag(flags::Flag("aaa", flags::String::Make("")));

  flags.Parse(std::vector<std::string>{"--aaa", "123"});

  auto ctx = pineapple::Context(flags);

  EXPECT_THROW(ctx.Flag("bbb"), pineapple::Exception);
}