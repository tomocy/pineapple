#include "src/pineapple/pineapple.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
Command::Command(const std::string& name, const std::string& description,
                 const pineapple::action_t& action)
    : name(ValidateName(name)),
      description(ValidateDescription(description)),
      action(ValidateAction(action)) {}

const std::string& Command::Name() const noexcept { return name; }

std::string Command::Usage() const noexcept { return Outline(); }

std::string Command::Outline() const noexcept {
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

const pineapple::action_t& Command::ValidateAction(
    const action_t& action) const {
  if (action == nullptr) {
    throw Exception("action should not be null");
  }

  return action;
}
}  // namespace pineapple

namespace pineapple {
App::App(const std::string& name) : App(name, "", nullptr) {}

App::App(const std::string& name, const std::string& description)
    : App(name, description, nullptr) {}

App::App(const std::string& name, const action_t& action)
    : App(name, "", action) {}

App::App(const std::string& name, const std::string& description,
         const action_t& action)
    : name(ValidateName(name)),
      description(ValidateDescription(description)),
      action(action),
      flags(flags::FlagSet(name)),
      commands(std::map<std::string, Command>()) {}

std::string App::Usage() const noexcept {
  auto usage = name;

  if (!description.empty()) {
    usage += " - " + description;
  }

  if (!commands.empty()) {
    usage += "\n";
    usage += "Commands:\n";

    for (auto [_, cmd] : commands) {
      usage += cmd.Outline() + "\n";
    }

    usage = usage.erase(usage.size() - 1, 1);
  }

  return usage;
}

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

  auto flags = flags::FlagSet(name);

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  flags.Parse(trimmed);

  auto parsed = flags.Args();

  if (parsed.size() < 1) {
    DoAction(parsed);
    return;
  }

  if (DoHaveCommand(parsed.at(0))) {
    RunCommand(parsed);
    return;
  }

  if (action != nullptr) {
    DoAction(parsed);
    return;
  }

  throw Exception("argument\"" + parsed.at(0) +
                  "\" is not handled at all: action or command named \"" +
                  parsed.at(0) + "\" is needed");
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

void App::DoAction(const std::vector<std::string>& args) const {
  if (action == nullptr) {
    return;
  }

  action(args);
}

void App::RunCommand(const std::vector<std::string>& args) const {
  if (args.size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto name = args.at(0);

  if (!DoHaveCommand(name)) {
    throw Exception("unknown command: command \"" + name + "\" is not added");
  }

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));

  commands.at(name).Run(trimmed);
}

bool App::DoHaveCommand(const std::string& name) const noexcept {
  return commands.find(name) != commands.end();
}
}  // namespace pineapple