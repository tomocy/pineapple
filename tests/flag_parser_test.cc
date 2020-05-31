#include "src/flag_parser.h"

#include <vector>

#include "external/gtest/googletest/include/gtest/gtest.h"

TEST(ReadToken, Empty) {
  auto src = std::vector<char>{};
  auto expected = std::vector<pineapple::Token>{
      pineapple::kTokenEOF,
  };

  auto lex = pineapple::Lexer(src);

  for (auto expected : expected) {
    auto actual = lex.ReadToken();
    EXPECT_EQ(actual.Kind(), expected.Kind());
    EXPECT_EQ(actual.Literal(), expected.Literal());
  }
}