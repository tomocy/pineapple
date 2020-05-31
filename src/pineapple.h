#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <string>
#include <vector>

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void()>;

  Command(const std::string& name, const std::string& description) noexcept;

  Command(const std::string& name, const std::string& description,
          const action_t& action) noexcept;

  void AddCommand(const Command& cmd) noexcept;

  void PrintHelp() const noexcept;

  std::string Help() const noexcept;

 private:
  Command(const std::string& name, const std::string& description,
          const action_t& action, const std::vector<Command>& commands) noexcept;

  std::string CommandsHelp() const noexcept;

  std::string name;
  std::string description;
  action_t action;
  std::vector<Command> commands;
};
}  // namespace pineapple
#endif