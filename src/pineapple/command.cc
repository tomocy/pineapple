#include "src/pineapple/command.h"

#include <iostream>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/context.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
Command::Command() noexcept : Command("command") {}

Command::Command(const std::string& name)
    : Command(name, description, nullptr) {}

Command::Command(const std::string& name, const std::string& description)
    : Command(name, description, nullptr) {}

Command::Command(const std::string& name, const std::string& description,
                 const Command::action_t& action)
    : flags(flags::FlagSet("flags")),
      action(action),
      name(ValidateName(name)),
      description(description),
      commands(std::map<std::string, Command>()) {}

const std::string& Command::Name() const noexcept { return name; }

std::string Command::Usage() const noexcept {
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

std::string Command::Outline() const noexcept {
  auto usage = name;
  if (!description.empty()) {
    usage += "  " + description;
  }

  return usage;
}

void Command::AddFlag(flags::Flag&& flag) { flags.AddFlag(std::move(flag)); }

void Command::AddCommand(Command&& command) {
  if (commands.find(command.Name()) != commands.end()) {
    throw Exception("command \"" + command.Name() + "\" is already added");
  }

  commands.emplace(command.Name(), std::move(command));
}

void Command::Run(Context&& parent) {
  if (parent.Args().size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  std::cout << "parent args:" << std::endl;
  for (auto arg : parent.Args()) {
    std::cout << arg << std::endl;
  }

  auto trimmed = std::vector<std::string>(std::begin(parent.Args()) + 1,
                                          std::end(parent.Args()));

  std::cout << "trimmed args:" << std::endl;
  for (auto arg : trimmed) {
    std::cout << arg << std::endl;
  }

  flags.Parse(trimmed);

  std::cout << "flags args:" << std::endl;
  for (auto arg : flags.Args()) {
    std::cout << arg << std::endl;
  }

  auto ctx = Context(Context::Make(std::move(parent)), flags);

  if (ctx.Args().size() >= 1 && DoHaveCommand(ctx.Args().at(0))) {
    RunCommand(std::move(ctx));
    return;
  }

  if (ctx.Args().empty() || action != nullptr) {
    DoAction(ctx);
    return;
  }

  throw Exception("argument\"" + ctx.Args().at(0) +
                  "\" is not handled at all: action or command named \"" +
                  ctx.Args().at(0) + "\" is needed");
}

const std::string& Command::ValidateName(const std::string& name) const {
  if (name.empty()) {
    throw Exception("name should not be empty");
  }

  return name;
}

std::string Command::FlagsUsage() const noexcept {
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

std::string Command::CommandsUsage() const noexcept {
  auto usage = std::string("Commands:\n");

  for (auto iter = std::begin(commands); iter != std::end(commands); ++iter) {
    usage += iter->second.Outline() + "\n";
  }

  return usage.erase(usage.size() - 1, 1);
}

void Command::DoAction(const Context& ctx) const {
  if (action == nullptr) {
    return;
  }

  action(ctx);
}

void Command::RunCommand(Context&& ctx) {
  if (ctx.Args().size() < 1) {
    throw Exception("command name should be provided");
  }

  auto name = ctx.Args().at(0);

  if (!DoHaveCommand(name)) {
    throw Exception("unknown command: command \"" + name + "\" is not added");
  }

  commands.at(name).Run(std::move(ctx));
}

bool Command::DoHaveCommand(const std::string& name) const noexcept {
  return commands.find(name) != commands.end();
}
}  // namespace pineapple