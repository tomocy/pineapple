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

void Command::Run(const std::vector<std::string>& args) const noexcept {
  action();
}

void Command::AddCommand(const Command& cmd) noexcept {
  commands.push_back(cmd);
}

void Command::PrintHelp() const noexcept { std::cout << Help() << std::endl; }

std::string Command::Help() const noexcept {
  auto help = name + " - " + description;
  auto cmdsHelp = CommandsHelp();
  if (!cmdsHelp.empty()) {
    help += "\n\n" + cmdsHelp;
  }

  return help;
}

std::string Command::CommandsHelp() const noexcept {
  if (commands.size() == 0) {
    return "";
  }

  auto help = std::string("Commands:\n");

  for (auto cmd : commands) {
    help += cmd.name + "    " + cmd.description + "\n";
  }

  return help.erase(help.size() - 1, 1);
}
}  // namespace pineapple
