#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_APP_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_APP_H

#include <functional>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/command.h"
#include "src/pineapple/context.h"

namespace pineapple {
class App {
 public:
  using const_action_ctx_t = const Context&;

  using action_t = std::function<void(const_action_ctx_t ctx)>;

  App(const std::string& name);

  App(const std::string& name, const std::string& description);

  App(const std::string& name, const action_t& action);

  App(const std::string& name, const std::string& description,
      const action_t& action);

  std::string Usage() const noexcept;

  void AddFlag(flags::Flag&& flag);

  void AddCommand(const Command& command);

  void Run(const std::vector<std::string>& args) const;

 private:
  const std::string& ValidateName(const std::string& name) const;

  const std::string& ValidateDescription(const std::string& description) const;

  std::string FlagsUsage() const noexcept;

  std::string CommandsUsage() const noexcept;

  void DoAction(const Context& ctx) const;

  void RunCommand(const std::vector<std::string>& args) const;

  bool DoHaveCommand(const std::string& name) const noexcept;

  std::string name;
  std::string description;
  action_t action;
  flags::FlagSet flags;
  std::map<std::string, Command> commands;
};
}  // namespace pineapple

#endif