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
  Command(const std::string& name) noexcept;

  std::string Help() const noexcept;

 private:
  std::string name;
};
}  // namespace pineapple
#endif