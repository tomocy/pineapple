#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action) noexcept
    : name(name), description(description), action(action) {}

std::string Command::Help() const noexcept {
  return name + " - " + description;
}
}  // namespace pineapple
