#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
class Context {
 public:
  Context() = default;
};
}  // namespace pineapple

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void(const Context&)>;

  Command() = default;

  explicit Command(const std::string& name) noexcept;

  const std::string& Name() const noexcept;

  Command& WithAction(const action_t&) noexcept;

  const action_t& Action() const noexcept;

 private:
  std::string name;

  action_t action;
};
}  // namespace pineapple

namespace pineapple {
class App {
 public:
  App() = default;

  void AddCommand(const Command& cmd) noexcept;

  void Run(const std::vector<std::string>& args) const noexcept;

 private:
  std::tuple<Command, bool> FindCommand(const std::string& name) const noexcept;

  std::map<std::string, Command> commands;
};
}  // namespace pineapple
#endif