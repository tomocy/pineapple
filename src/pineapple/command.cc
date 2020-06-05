#include "src/pineapple/command.h"

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

void Command::Run(Context&& ctx) const {
  if (ctx.Args().size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto flags = flags::FlagSet(name);

  auto trimmed = std::vector<std::string>(std::begin(ctx.Args()) + 1,
                                          std::end(ctx.Args()));
  flags.Parse(trimmed);

  ctx = Context(std::move(ctx), std::move(flags));

  action(ctx);
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
}  // namespace pineapple