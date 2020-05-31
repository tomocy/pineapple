#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : Command(name, description, [this]() { this->PrintHelp(); }) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action) noexcept
    : Command(name, description, action, std::vector<Command>()) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action,
                 const std::vector<Command>& commands) noexcept
    : name(name),
      description(description),
      action(action),
      commands(commands) {}

void Command::AddCommand(const Command& cmd) noexcept {
  commands.push_back(cmd);
}

void Command::PrintHelp() const noexcept { std::cout << Help() << std::endl; }

std::string Command::Help() const noexcept {
  return name + " - " + description + "\n" + CommandsHelp();
}

std::string Command::CommandsHelp() const noexcept {
  if (commands.size() == 0) {
    return "";
  }

  auto help = std::string("\nCommands:\n");

  for (auto cmd : commands) {
    help += cmd.name + "    " + cmd.description + "\n";
  }

  return help;
}
}  // namespace pineapple
