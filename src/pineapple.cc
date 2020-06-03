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
Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action)
    : name(ValidateName(name)),
      description(ValidateDescription(description)),
      action(ValidateAction(action)) {}

const std::string& Command::Name() const noexcept { return name; }

std::string Command::Usage() const noexcept {
  auto usage = name;
  if (!description.empty()) {
    usage += "  " + description;
  }

  return usage;
}

void Command::Run(const std::vector<std::string>& args) const { action(args); }

const std::string& Command::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}

const std::string& Command::ValidateDescription(
    const std::string& description) const {
  return description;
}

const typename Command::action_t& Command::ValidateAction(
    const action_t& action) const {
  if (action == nullptr) {
    throw Exception("action should not be null");
  }

  return action;
}
}  // namespace pineapple

namespace pineapple {
App::App(const std::string& name, const std::string& description)
    : name(ValidateName(name)),
      description(ValidateDescription(description)),
      commands(std::map<std::string, Command>()) {}

void App::AddCommand(const Command& command) {
  if (commands.find(command.Name()) != commands.end()) {
    throw Exception("command \"" + command.Name() + "\" is already added");
  }

  commands.emplace(command.Name(), command);
}

void App::Run(const std::vector<std::string>& args) const {
  if (args.size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  if (trimmed.size() < 1) {
    return;
  }

  auto name = trimmed.at(0);
  if (commands.find(name) == commands.end()) {
    throw Exception("unknown command: command \"" + name + "\" is not added");
  }

  trimmed =
      std::vector<std::string>(std::begin(trimmed) + 1, std::end(trimmed));

  commands.at(name).Run(trimmed);
}

const std::string& App::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}

const std::string& App::ValidateDescription(
    const std::string& description) const {
  return description;
}
}  // namespace pineapple