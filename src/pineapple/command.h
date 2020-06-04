#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_COMMAND_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_COMMAND_H

#include <functional>
#include <string>
#include <vector>

#include "src/pineapple/context.h"

namespace pineapple {
class Command {
 public:
  using const_action_ctx_t = const Context&;

  using action_t = std::function<void(const_action_ctx_t ctx)>;

  Command() = default;

  Command(const std::string& name, const std::string& description,
          const action_t& action);

  const std::string& Name() const noexcept;

  std::string Usage() const noexcept;

  std::string Outline() const noexcept;

  void Run(Context&& ctx) const;

 private:
  const std::string& ValidateName(const std::string& name) const;

  const std::string& ValidateDescription(const std::string& descriptoin) const;

  const action_t& ValidateAction(const action_t& action) const;

  std::string name;
  std::string description;
  action_t action;
};
}  // namespace pineapple

#endif