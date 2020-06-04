#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_PINEAPPLE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
using action_t = std::function<void(const std::vector<std::string>& args)>;
}

namespace pineapple {
class Command {
 public:
  Command() = default;

  Command(const std::string& name, const std::string& description,
          const action_t& action);

  const std::string& Name() const noexcept;

  std::string Usage() const noexcept;

  std::string Outline() const noexcept;

  void Run(const std::vector<std::string>& args) const;

 private:
  const std::string& ValidateName(const std::string& name) const;

  const std::string& ValidateDescription(const std::string& descriptoin) const;

  const action_t& ValidateAction(const action_t& action) const;

  std::string name;
  std::string description;
  action_t action;
};
}  // namespace pineapple

namespace pineapple {
class App {
 public:
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

  void DoAction(const std::vector<std::string>& args) const;

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