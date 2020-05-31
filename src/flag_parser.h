#ifndef TOMOCY_FLAG_PARSER_H
#define TOMOCY_FLAG_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "src/flag_parser.h"

namespace pineapple {
enum class TokenKind {
  END_OF_FILE,
  UNKNOWN,
  SHORT_HYPHEN,
  LONG_HYPHEN,
  EQUAL,
  STRING,
};
}  // namespace pineapple

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
}  // namespace pineapple

namespace pineapple {
class Lexer {
 public:
  Lexer(const std::vector<char>& src) noexcept;

  Token ReadToken() noexcept;

 private:
  Token ComposeDualTokenAs(TokenKind kind) noexcept;

  Token ComposeSingleTokenAs(TokenKind kind) noexcept;

  Token ComposeTokenAs(size_t n, TokenKind kind) noexcept;

  Token ComposeString() noexcept;

  std::string ReadLetters() noexcept;

  bool DoHaveLetter() const noexcept;

  void SkipWhitespaces() noexcept;

  bool DoHaveWhitespace() const noexcept;

  bool WillHave(char c) const noexcept;

  void ReadChar() noexcept;

  char CurrentChar() const noexcept;

  char NextChar() const noexcept;

  std::vector<char> src;
  size_t index;
};

const char kCharEOF = 0;
}  // namespace pineapple

#endif