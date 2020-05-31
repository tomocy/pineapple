#include "src/pineapple.h"

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
Command::Command(const std::string& name) noexcept : name(name) {}

const std::string& Command::Name() const noexcept { return name; }

Command& Command::WithAction(
    const typename Command::action_t& action) noexcept {
  this->action = action;
  return *this;
}

const typename Command::action_t& Command::Action() const noexcept {
  return action;
}
}  // namespace pineapple

namespace pineapple {
void App::AddCommand(const ::pineapple::Command& cmd) noexcept {
  commands[cmd.Name()] = cmd;
}

void App::Run(const std::vector<std::string>& args) const noexcept {
  if (args.size() < 2) {
    return;
  }

  auto name = args.at(1);
  auto [cmd, found] = FindCommand(name);
  if (!found) {
    // TODO: show help
    return;
  }

  cmd.Action()(Context());
}

std::tuple<Command, bool> App::FindCommand(const std::string& name) const
    noexcept {
  if (commands.find(name) == commands.end()) {
    return {Command(), false};
  }

  return {commands.at(name), true};
}
}  // namespace pineapple