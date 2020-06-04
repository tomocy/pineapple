#include "src/pineapple/exceptions.h"

#include <string>

namespace pineapple {
Exception::Exception(const std::string& msg) noexcept : msg(msg) {}

const std::string& Exception::What() const noexcept { return msg; }
}  // namespace pineapple