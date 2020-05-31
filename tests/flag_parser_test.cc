#include "src/flag_parser.h"

#include <string>
#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

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