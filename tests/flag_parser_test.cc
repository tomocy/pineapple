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