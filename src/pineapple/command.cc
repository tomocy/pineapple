#include "src/pineapple/command.h"

#include <iostream>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/context.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
Command::Command() noexcept
    : Command("command", "description", [](const_action_ctx_t _) {}) {}

Command::Command(const std::string& name, const std::string& description,
                 const Command::action_t& action)
    : name(ValidateName(name)),
      description(ValidateDescription(description)),
      flags(flags::FlagSet("flags")),
      action(ValidateAction(action)),
      commands(std::map<std::string, Command>()) {}

const std::string& Command::Name() const noexcept { return name; }

std::string Command::Usage() const noexcept { return Outline(); }

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

void Command::Run(Context&& ctx) {
  if (ctx.Args().size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto trimmed = std::vector<std::string>(std::begin(ctx.Args()) + 1,
                                          std::end(ctx.Args()));
  flags.Parse(trimmed);

  ctx = Context(std::move(ctx), std::move(flags));

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

const std::string& Command::ValidateDescription(
    const std::string& description) const {
  return description;
}

const typename Command::action_t& Command::ValidateAction(
    const Command::action_t& action) const {
  if (action == nullptr) {
    throw Exception("action should not be null");
  }

  return action;
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