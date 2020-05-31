#ifndef TOMOCY_FLAG_PARSER_H
#define TOMOCY_FLAG_PARSER_H

#include <string>
#include <vector>

namespace pineapple {
enum class TokenKind {};
}

namespace pineapple {
class Token {
 public:
  Token(TokenKind kind, const std::string& literal) noexcept;

 private:
  TokenKind kind;
  std::string literal;
};
}  // namespace pineapple

namespace pineapple {
class Lexer {
 public:
  Lexer(const std::vector<char>& src) noexcept;

 private:
  std::vector<char> src;
};
}  // namespace pineapple

#endif