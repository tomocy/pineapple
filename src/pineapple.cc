#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : name(name), description(description) {}

std::string Command::Help() const noexcept {
  return name + " - " + description;
}
}  // namespace pineapple
