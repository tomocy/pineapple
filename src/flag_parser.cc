#include "src/flag_parser.h"

#include <string>
#include <vector>

namespace pineapple {
Token::Token(TokenKind kind, const std::string& literal) noexcept
    : kind(kind), literal(literal) {}
}  // namespace pineapple

namespace pineapple {
Lexer::Lexer(const std::vector<char>& src) noexcept : src(src) {}
}  // namespace pineapple