#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H

#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"

namespace pineapple {
class Context {
 public:
  Context(flags::FlagSet&& flags) noexcept;

  const flags::Flag& Flag(const std::string& name) const;

  const std::vector<std::string>& Args() const noexcept;

 private:
  flags::FlagSet flags;
};
}  // namespace pineapple

#endif