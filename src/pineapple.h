#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <string>

namespace pineapple {
using command_action_t = std::function<void()>;

class Command {
 public:
  using action_t = command_action_t;

  Command(const std::string& name, const std::string& description,
          const action_t& action) noexcept;

  std::string Help() const noexcept;

 private:
  std::string name;
  std::string description;
  action_t action;
};
}  // namespace pineapple
#endif