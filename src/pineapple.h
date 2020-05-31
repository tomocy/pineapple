#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <string>

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void()>;

  Command(const std::string& name, const std::string& description) noexcept;

  Command(const std::string& name, const std::string& description,
          const action_t& action) noexcept;

  void PrintHelp() const noexcept;

  std::string Help() const noexcept;

 private:
  std::string name;
  std::string description;
  action_t action;
};
}  // namespace pineapple
#endif