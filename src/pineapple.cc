#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <string>

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : Command(name, description, [this]() { this->PrintHelp(); }) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action) noexcept
    : name(name), description(description), action(action) {}

void Command::PrintHelp() const noexcept { std::cout << Help() << std::endl; }

std::string Command::Help() const noexcept {
  return name + " - " + description;
}
}  // namespace pineapple
