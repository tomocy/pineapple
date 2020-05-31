#include "src/flag_parser.h"

#include <iostream>
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
  SkipWhitespaces();

  auto curr = CurrentChar();

  if (curr == kCharEOF) {
    return ComposeSingleTokenAs(TokenKind::END_OF_FILE);
  }

  if (curr == '-') {
    return ComposeSingleTokenAs(TokenKind::SHORT_HYPHEN);
  }

  if (DoHaveLetter()) {
    return ComposeString();
  }

  return ComposeSingleTokenAs(TokenKind::UNKNOWN);
}

Token Lexer::ComposeSingleTokenAs(TokenKind kind) noexcept {
  auto tok = Token(kind, std::string{CurrentChar()});

  if (kind == TokenKind::END_OF_FILE) {
    tok = kTokenEOF;
  }

  ReadChar();

  return tok;
}

Token Lexer::ComposeString() noexcept {
  auto literal = ReadLetters();
  return Token(TokenKind::STRING, literal);
}

std::string Lexer::ReadLetters() noexcept {
  auto begin = index;
  while (DoHaveLetter()) {
    ReadChar();
  }

  return std::string(std::begin(src) + begin, std::begin(src) + index);
}

bool Lexer::DoHaveLetter() const noexcept {
  auto curr = CurrentChar();
  return ('a' <= curr && curr <= 'z') || ('A' <= curr && curr <= 'Z') ||
         ('0' <= curr && curr <= '9');
}

void Lexer::SkipWhitespaces() noexcept {
  while (DoHaveWhitespace()) {
    ReadChar();
  }
}

bool Lexer::DoHaveWhitespace() const noexcept {
  auto curr = Lexer::CurrentChar();
  return curr == ' ' || curr == '\t';
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