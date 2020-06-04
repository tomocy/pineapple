#include "src/pineapple/context.h"

#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"

namespace pineapple {
Context::Context(flags::FlagSet&& flags) noexcept : flags(std::move(flags)) {}

const std::vector<std::string>& Context::Args() const noexcept {
  return flags.Args();
}
}  // namespace pineapple