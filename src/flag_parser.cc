#include "src/flag_parser.h"

#include <string>
#include <vector>

namespace pineapple {
Token::Token(TokenKind kind, const std::string& literal) noexcept
    : kind(kind), literal(literal) {}

TokenKind Token::Kind() const noexcept { return kind; }

const std::string& Token::Literal() const noexcept { return literal; }
}  // namespace pineapple

namespace pineapple {
Lexer::Lexer(const std::vector<char>& src) noexcept : src(src), index(0) {}

Token Lexer::ReadToken() noexcept {
  auto curr = CurrentChar();
  if (curr == kCharEOF) {
    return kTokenEOF;
  }

  return kTokenUnknown;
}

void Lexer::ReadChar() noexcept {
  if (index >= src.size()) {
    return;
  }

  ++index;
}

char Lexer::CurrentChar() const noexcept {
  if (index >= src.size()) {
    return kCharEOF;
  }

  return src.at(index);
}
}  // namespace pineapple