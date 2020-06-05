#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_COMMAND_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_COMMAND_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/context.h"

namespace pineapple {
class Command {
 public:
  using const_action_ctx_t = const Context&;

  using action_t = std::function<void(const_action_ctx_t ctx)>;

  Command() noexcept;

  Command(const std::string& name);

  Command(const std::string& name, const std::string& description);

  Command(const std::string& name, const std::string& description,
          const action_t& action);

  const std::string& Name() const noexcept;

  std::string Usage() const noexcept;

  std::string Outline() const noexcept;

  void AddFlag(flags::Flag&& flag);

  void AddCommand(Command&& command);

  void Run(Context&& ctx);

 private:
  const std::string& ValidateName(const std::string& name) const;

  std::string FlagsUsage() const noexcept;

  std::string CommandsUsage() const noexcept;

  void DoAction(const Context& ctx) const;

  void RunCommand(Context&& ctx);

  bool DoHaveCommand(const std::string& name) const noexcept;

  std::string name;
  std::string description;
  flags::FlagSet flags;
  action_t action;
  std::map<std::string, Command> commands;
};
}  // namespace pineapple

#endif