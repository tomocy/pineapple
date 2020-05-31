#ifndef TOMOCY_FLAG_PARSER_H
#define TOMOCY_FLAG_PARSER_H

#include <string>
#include <vector>

namespace pineapple {
enum class TokenKind { END_OF_FILE, UNKNOWN };
}

namespace pineapple {
class Token {
 public:
  Token(TokenKind kind, const std::string& literal) noexcept;

  TokenKind Kind() const noexcept;

  const std::string& Literal() const noexcept;

 private:
  TokenKind kind;
  std::string literal;
};

const Token kTokenEOF = Token(TokenKind::END_OF_FILE, "\x00");
const Token kTokenUnknown = Token(TokenKind::UNKNOWN, "");
}  // namespace pineapple

namespace pineapple {
class Lexer {
 public:
  Lexer(const std::vector<char>& src) noexcept;

  Token ReadToken() noexcept;

 private:
  void ReadChar() noexcept;

  char CurrentChar() const noexcept;

  std::vector<char> src;
  size_t index;
};

const char kCharEOF = 0;
}  // namespace pineapple

#endif