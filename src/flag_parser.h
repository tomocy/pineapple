#ifndef TOMOCY_FLAG_PARSER_H
#define TOMOCY_FLAG_PARSER_H

#include <vector>

namespace pineapple {
class Lexer {
 public:
  Lexer(const std::vector<char>& src) noexcept;

 private:
  std::vector<char> src;
};
}  // namespace pineapple

#endif