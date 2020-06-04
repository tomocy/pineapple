#include "src/pineapple/context.h"

#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"

namespace pineapple {
Context::Context(Context&& parent, flags::FlagSet&& flags) noexcept
    : parent(std::make_unique<Context>(std::move(parent))),
      flags(std::move(flags)) {}

Context::Context(flags::FlagSet&& flags) noexcept : flags(std::move(flags)) {}

const typename Context::parent_t& Context::Parent() const noexcept {
  return parent;
}

const flags::Flag& Context::Flag(const std::string& name) const {
  return flags.GetFlag(name);
}

const std::vector<std::string>& Context::Args() const noexcept {
  return flags.Args();
}
}  // namespace pineapple