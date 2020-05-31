#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
StringFlag::StringFlag(const std::string& name,
                       const std::string& value) noexcept
    : name(name), value(value) {}

const std::string& StringFlag::Name() const noexcept { return name; }

const std::string& StringFlag::Value() const noexcept { return value; }
}  // namespace pineapple

namespace pineapple {
FlagSet::FlagSet(const std::vector<Flag>& flags) noexcept : flags(flags) {}
}  // namespace pineapple

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : Command(name, description, [this](auto _) { this->PrintHelp(); }) {}

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
  if (args.size() < 1) {
    return;
  }

  if (commands.size() >= 1 && args.size() >= 2) {
    auto name = args.at(1);
    auto sliced =
        std::vector<std::string>(std::begin(args) + 2, std::end(args));
    RunAsCommand(name, sliced);

    return;
  }

  auto sliced = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  action(sliced);
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

void Command::RunAsCommand(const std::string& name,
                           const std::vector<std::string>& args) const
    noexcept {
  auto [cmd, found] = FindCommand(name);
  if (!found) {
    PrintHelp();
    return;
  }

  cmd.Run(args);
}

std::tuple<Command, bool> Command::FindCommand(const std::string& name) const
    noexcept {
  for (auto cmd : commands) {
    if (cmd.name == name) {
      return {cmd, true};
    }
  }

  return {Command(name, ""), false};
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