#include "src/pineapple/app.h"

#include <map>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/command.h"
#include "src/pineapple/context.h"
#include "src/pineapple/exceptions.h"

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

void App::Run(const std::vector<std::string>& args) {
  if (args.size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  flags.Parse(trimmed);

  auto ctx = Context(std::move(flags));

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

void App::DoAction(const Context& ctx) const {
  if (action == nullptr) {
    return;
  }

  action(ctx);
}

void App::RunCommand(Context&& ctx) const {
  if (ctx.Args().size() < 1) {
    throw Exception("command name should be provided");
  }

  auto name = ctx.Args().at(0);

  if (!DoHaveCommand(name)) {
    throw Exception("unknown command: command \"" + name + "\" is not added");
  }

  commands.at(name).Run(std::move(ctx));
}

bool App::DoHaveCommand(const std::string& name) const noexcept {
  return commands.find(name) != commands.end();
}
}  // namespace pineapple