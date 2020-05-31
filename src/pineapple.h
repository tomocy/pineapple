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
  Command(const std::string& name, const std::string& description) noexcept;

  std::string Help() const noexcept;

 private:
  std::string name;
  std::string description;
};
}  // namespace pineapple
#endif