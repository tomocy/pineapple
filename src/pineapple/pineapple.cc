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

void Command::Run(const std::vector<std::string>& args) const {
  if (args.size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto flags = flags::FlagSet(name);

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  flags.Parse(trimmed);

  action(flags.Args());
}

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

  if (flags.FlagSize() >= 1) {
    usage += "\n";
    usage += FlagsUsage();
  }

  if (!commands.empty()) {
    usage += "\n";
    usage += CommandsUsage();
  }

  return usage;
}

void App::AddFlag(flags::Flag&& flag) { flags.AddFlag(std::move(flag)); }

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

  if (parsed.size() >= 1 && DoHaveCommand(parsed.at(0))) {
    RunCommand(parsed);
    return;
  }

  if (parsed.empty() || action != nullptr) {
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

std::string App::FlagsUsage() const noexcept {
  auto usage = std::string("Flags:\n");

  usage += flags.Usage([](const auto& _, const auto& flags) {
    auto usage = std::string("");

    for (auto iter = std::begin(flags); iter != std::end(flags); ++iter) {
      usage += iter->second.Usage() + "\n";
    }

    return usage;
  });

  return usage.erase(usage.size() - 1, 1);
}

std::string App::CommandsUsage() const noexcept {
  auto usage = std::string("Commands:\n");

  for (auto [_, cmd] : commands) {
    usage += cmd.Outline() + "\n";
  }

  return usage.erase(usage.size() - 1, 1);
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

  commands.at(name).Run(args);
}

bool App::DoHaveCommand(const std::string& name) const noexcept {
  return commands.find(name) != commands.end();
}
}  // namespace pineapple