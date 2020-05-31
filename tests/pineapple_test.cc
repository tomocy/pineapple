#include "src/pineapple.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(Parse, Empty) {
  auto src = std::string("");
  auto expected = std::vector<pineapple::Flag>{};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, String) {
  auto src = std::string("d");
  auto expected = std::vector<std::string>{"d"};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Args();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i), expected.at(i));
  }
}

TEST(Parse, ShortFlag) {
  auto src = std::string("-d");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, ShortFlagWithValue) {
  auto src = std::string("-d a");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "a")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, ShortFlagWithEqualValue) {
  auto src = std::string("-d=a");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "a")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, LongFlag) {
  auto src = std::string("--d");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, LongFlagWithValue) {
  auto src = std::string("--d a");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "a")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, LongFlagWithEqualValue) {
  auto src = std::string("--d=a");
  auto expected = std::vector<pineapple::Flag>{pineapple::Flag("d", "a")};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual = parser.Flags();

  ASSERT_EQ(actual.size(), expected.size());

  for (auto i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(actual.at(i).Name(), expected.at(i).Name());
    EXPECT_EQ(actual.at(i).Value(), expected.at(i).Value());
  }
}

TEST(Parse, Real) {
  auto src = std::string("--a aiueo --b -c=ccc x y z");
  auto expected_flags = std::vector<pineapple::Flag>{
      pineapple::Flag("a", "aiueo"), pineapple::Flag("b", ""),
      pineapple::Flag("c", "ccc")};
  auto expected_args = std::vector<std::string>{"x", "y", "z"};

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));
  auto parser = pineapple::Parser(lex);

  parser.Parse();

  auto actual_flags = parser.Flags();

  ASSERT_EQ(actual_flags.size(), expected_flags.size());

  for (auto i = 0; i < expected_flags.size(); ++i) {
    EXPECT_EQ(actual_flags.at(i).Name(), expected_flags.at(i).Name());
    EXPECT_EQ(actual_flags.at(i).Value(), expected_flags.at(i).Value());
  }

  auto actual_args = parser.Args();

  ASSERT_EQ(actual_args.size(), expected_args.size());

  for (auto i = 0; i < expected_args.size(); ++i) {
    EXPECT_EQ(actual_args.at(i), expected_args.at(i));
  }
}

TEST(FlagSet, Parse) {
  auto src = std::vector<std::string>{"--a",     "aiueo", "--b", "-c",
                                      "-d=ddee", "x",     "y",   "z"};
  auto expected_flags = std::vector<pineapple::Flag>{
      pineapple::Flag("a", "aiueo"),
      pineapple::Flag("b", ""),
      pineapple::Flag("c", "ccc"),
      pineapple::Flag("d", "ddee"),
  };
  auto expected_args = std::vector<std::string>{"x", "y", "z"};

  auto flags = pineapple::FlagSet(expected_flags);

  flags.Parse(src);

  for (auto expected : expected_flags) {
    EXPECT_EQ(flags.Get(expected.Name()), expected.Value());
  }

  auto actual_args = flags.Args();

  ASSERT_EQ(actual_args.size(), expected_args.size());

  for (auto i = 0; i < expected_args.size(); ++i) {
    EXPECT_EQ(actual_args.at(i), expected_args.at(i));
  }
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