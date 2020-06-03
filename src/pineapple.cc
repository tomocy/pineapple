#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "external/flags/src/flags.h"
#include "src/exceptions.h"

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : Command(name, description, [this](auto _) { this->PrintHelp(); }) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action) noexcept
    : name(name),
      description(description),
      action(action),
      commands(std::map<std::string, Command>()) {}

void Command::Run(const std::vector<std::string>& args) const noexcept {
  if (args.size() < 1) {
    return;
  }

  auto name = args.at(0);

  auto flags = flags::FlagSet(name);

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));

  if (commands.size() >= 1) {
    RunAsSubcommand(trimmed);
    return;
  }

  flags.Parse(trimmed);

  action(flags.Args());

  return;
}

void Command::AddCommand(const Command& command) {
  if (commands.find(command.name) != commands.end()) {
    throw Exception("command \"" + command.name + "\" is already added");
  }

  commands[command.name] = command;
}

void Command::PrintHelp() const noexcept { std::cout << Help() << std::endl; }

std::string Command::Help() const noexcept {
  auto help = name + " - " + description + "\n";

  auto cmdsHelp = HelpOfSubcommands();
  if (!cmdsHelp.empty()) {
    help += cmdsHelp;
  }

  return help;
}

void Command::RunAsSubcommand(const std::vector<std::string>& args) const
    noexcept {
  if (args.size() < 1) {
    return;
  }

  auto name = args.at(0);
  auto [cmd, found] = FindCommand(name);
  if (!found) {
    return;
  }

  cmd.Run(args);
}

std::tuple<Command, bool> Command::FindCommand(const std::string& name) const
    noexcept {
  if (commands.find(name) == commands.end()) {
    return {Command(), false};
  }

  return {commands.at(name), true};
}

std::string Command::HelpOfSubcommands() const noexcept {
  if (commands.size() == 0) {
    return "";
  }

  auto help = std::string("Commands:\n");

  for (auto [_, cmd] : commands) {
    help += cmd.HelpAsSubcommand() + "\n";
  }

  return help.erase(help.size() - 1, 1);
}

std::string Command::HelpAsSubcommand() const noexcept {
  return name + "  " + description;
}
}  // namespace pineapple