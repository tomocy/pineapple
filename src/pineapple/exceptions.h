#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_EXCEPTIONS_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_EXCEPTIONS_H

#include <string>

namespace pineapple {
class Exception {
 public:
  explicit Exception(const std::string& msg) noexcept;

  const std::string& What() const noexcept;

 private:
  std::string msg;
};
}  // namespace pineapple

#endif