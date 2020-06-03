#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void(const std::vector<std::string>& args)>;

  Command() = default;

  Command(const std::string& name, const std::string& description) noexcept;

  Command(const std::string& name, const std::string& description,
          const action_t& action) noexcept;

  void Run(const std::vector<std::string>& args) const noexcept;

  void AddCommand(const Command& command);

  void PrintHelp() const noexcept;

  std::string Help() const noexcept;

 private:
  void RunAsSubcommand(const std::vector<std::string>& args) const noexcept;

  std::tuple<Command, bool> FindCommand(const std::string& name) const noexcept;

  std::string HelpOfSubcommands() const noexcept;

  std::string HelpAsSubcommand() const noexcept;

  std::string name;
  std::string description;
  action_t action;
  std::map<std::string, Command> commands;
};
}  // namespace pineapple

#endif