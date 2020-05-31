#include "src/flag_parser.h"

#include <vector>

namespace pineapple {
Lexer::Lexer(const std::vector<char>& src) noexcept : src(src) {}
}  // namespace pineapple