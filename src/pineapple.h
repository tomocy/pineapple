#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "src/exceptions.h"

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void(const std::vector<std::string>& args)>;

  Command() = default;

  Command(const std::string& name, const std::string& description,
          const action_t& action);

  const std::string& Name() const noexcept;

  std::string Usage() const noexcept;

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
  App(const std::string& name, const std::string& description);

  void AddCommand(const Command& command);

  void Run(const std::vector<std::string>& args) const;

 private:
  const std::string& ValidateName(const std::string& name) const;

  const std::string& ValidateDescription(const std::string& description) const;

  std::string name;
  std::string description;
  std::map<std::string, Command> commands;
};
}  // namespace pineapple

#endif