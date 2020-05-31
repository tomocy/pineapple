#include "src/flag_parser.h"

#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"
#include "src/pineapple.h"

TEST(ReadToken, Empty) {
  auto src = std::string("");
  auto expected = std::vector<pineapple::Token>{
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, String) {
  auto src = std::string("aiueo");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::STRING, "aiueo"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, SomeStrings) {
  auto src = std::string("aiueo 1 a b c 1234");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::STRING, "aiueo"),
      pineapple::Token(pineapple::TokenKind::STRING, "1"),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::Token(pineapple::TokenKind::STRING, "b"),
      pineapple::Token(pineapple::TokenKind::STRING, "c"),
      pineapple::Token(pineapple::TokenKind::STRING, "1234"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, ShortFlag) {
  auto src = std::string("-d");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::SHORT_HYPHEN, "-"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, ShortFlagWithValue) {
  auto src = std::string("-d a");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::SHORT_HYPHEN, "-"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, ShortFlagWithEqualValue) {
  auto src = std::string("-d=a");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::SHORT_HYPHEN, "-"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::Token(pineapple::TokenKind::EQUAL, "="),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, LongFlag) {
  auto src = std::string("--d");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::LONG_HYPHEN, "--"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, LongFlagWithValue) {
  auto src = std::string("--d a");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::LONG_HYPHEN, "--"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, LongFlagWithEqualValue) {
  auto src = std::string("--d=a");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::LONG_HYPHEN, "--"),
      pineapple::Token(pineapple::TokenKind::STRING, "d"),
      pineapple::Token(pineapple::TokenKind::EQUAL, "="),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(ReadToken, Real) {
  auto src = std::string("--a aiueo --b -c=ccc x y z");
  auto expected = std::vector<pineapple::Token>{
      pineapple::Token(pineapple::TokenKind::LONG_HYPHEN, "--"),
      pineapple::Token(pineapple::TokenKind::STRING, "a"),
      pineapple::Token(pineapple::TokenKind::STRING, "aiueo"),
      pineapple::Token(pineapple::TokenKind::LONG_HYPHEN, "--"),
      pineapple::Token(pineapple::TokenKind::STRING, "b"),
      pineapple::Token(pineapple::TokenKind::SHORT_HYPHEN, "-"),
      pineapple::Token(pineapple::TokenKind::STRING, "c"),
      pineapple::Token(pineapple::TokenKind::EQUAL, "="),
      pineapple::Token(pineapple::TokenKind::STRING, "ccc"),
      pineapple::Token(pineapple::TokenKind::STRING, "x"),
      pineapple::Token(pineapple::TokenKind::STRING, "y"),
      pineapple::Token(pineapple::TokenKind::STRING, "z"),
      pineapple::kTokenEOF,
  };

  auto lex =
      pineapple::Lexer(std::vector<char>(std::begin(src), std::end(src)));

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}

TEST(Parse, Empty) {
  auto src = std::string("");
  auto expected = std::vector<pineapple::StringFlag>{};

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
  auto expected =
      std::vector<pineapple::StringFlag>{pineapple::StringFlag("d", "")};

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