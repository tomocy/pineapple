#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
class Flag {
 public:
  Flag() = default;

  Flag(const std::string& name) noexcept;

  Flag(const std::string& name, const std::string& value) noexcept;

  const std::string& Name() const noexcept;

  const std::string& Value() const noexcept;

 private:
  std::string name;

  std::string value;
};
}  // namespace pineapple

namespace pineapple {
class Context {
 public:
  Context() = default;

  Context(const std::map<std::string, Flag>& flags,
          const std::vector<std::string>& args) noexcept;

  std::string Get(const std::string& name) const noexcept;

  const std::vector<std::string>& Args() const noexcept;

 private:
  std::tuple<Flag, bool> Find(const std::string& name) const noexcept;

  std::map<std::string, Flag> flags;

  std::vector<std::string> args;
};
}  // namespace pineapple

namespace pineapple {
class Parser {
 public:
  Parser() = default;

  Context Parse(const std::vector<std::string>& args) const noexcept;

 private:
  bool IsFlagName(const std::string& name) const noexcept;

  std::string TrimFlagName(const std::string& name) const noexcept;
};
}  // namespace pineapple

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void(const Context&)>;

  Command() = default;

  explicit Command(const std::string& name) noexcept;

  const std::string& Name() const noexcept;

  Command& WithAction(const action_t& action) noexcept;

  void Run(const std::vector<std::string>& args) const noexcept;

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