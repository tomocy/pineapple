#include "src/flag_parser.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/pineapple.h"

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
    if (WillHave('-')) {
      return ComposeDualTokenAs(TokenKind::LONG_HYPHEN);
    }

    return ComposeSingleTokenAs(TokenKind::SHORT_HYPHEN);
  }

  if (curr == '=') {
    return ComposeSingleTokenAs(TokenKind::EQUAL);
  }

  if (DoHaveLetter()) {
    return ComposeString();
  }

  return ComposeSingleTokenAs(TokenKind::UNKNOWN);
}

Token Lexer::ComposeDualTokenAs(TokenKind kind) noexcept {
  return ComposeTokenAs(2, kind);
}

Token Lexer::ComposeSingleTokenAs(TokenKind kind) noexcept {
  if (kind == TokenKind::END_OF_FILE) {
    ReadChar();
    return kTokenEOF;
  }

  return ComposeTokenAs(1, kind);
}

Token Lexer::ComposeTokenAs(size_t n, TokenKind kind) noexcept {
  auto literal = std::string("");
  for (auto i = 0; i < n; ++i) {
    literal += std::string{CurrentChar()};
    ReadChar();
  }

  return Token(kind, literal);
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

bool Lexer::WillHave(char c) const noexcept {
  auto next = Lexer::NextChar();
  return next == c;
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

char Lexer::NextChar() const noexcept {
  auto nextIndex = index + 1;

  if (nextIndex >= src.size()) {
    return kCharEOF;
  }

  return src.at(nextIndex);
}
}  // namespace pineapple

namespace pineapple {
Parser::Parser(const Lexer& lexer) noexcept
    : lexer(lexer),
      currToken(kTokenEOF),
      nextToken(kTokenEOF),
      flags(std::vector<StringFlag>()),
      args(std::vector<std::string>()) {
  ReadToken();
  ReadToken();
}

void Parser::Parse() noexcept {
  while (true) {
    switch (currToken.Kind()) {
      case TokenKind::END_OF_FILE:
      case TokenKind::UNKNOWN:
        return;
      case TokenKind::SHORT_HYPHEN:
        ParseFlag();
        break;
      case TokenKind::STRING:
        ParseArg();
        break;
    }
  }
}

const std::vector<StringFlag>& Parser::Flags() const noexcept { return flags; }

const std::vector<std::string>& Parser::Args() const noexcept { return args; }

void Parser::ParseFlag() noexcept {
  ReadToken();

  auto name = ParseString();

  if (DoHave(TokenKind::EQUAL)) {
    ReadToken();
  }

  if (DoHave(TokenKind::SHORT_HYPHEN) || DoHave(TokenKind::LONG_HYPHEN)) {
    flags.push_back(StringFlag(name, ""));
    return;
  }

  auto value = ParseString();
  flags.push_back(StringFlag(name, value));
}

void Parser::ParseArg() noexcept {
  auto arg = ParseString();
  args.push_back(arg);
}

std::string Parser::ParseString() noexcept {
  auto literal = currToken.Literal();

  ReadToken();

  return literal;
}

bool Parser::DoHave(TokenKind kind) const noexcept {
  return currToken.Kind() == kind;
}

void Parser::ReadToken() noexcept {
  currToken = nextToken;
  nextToken = lexer.ReadToken();
}
}  // namespace pineapple