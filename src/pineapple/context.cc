#include "src/pineapple/context.h"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
typename Context::parent_t Context::Make(Context&& ctx) noexcept {
  return std::make_unique<Context>(std::move(ctx));
}

Context::Context(const flags::FlagSet& flags) noexcept
    : Context(nullptr, flags) {}

Context::Context(parent_t&& parent, const flags::FlagSet& flags) noexcept
    : parent(std::move(parent)), flags(flags) {}

const flags::Flag& Context::GlobalFlag(const std::string& name) const {
  auto [flag, ok] = TryToGetGlobalFlag(name);
  if (!ok) {
    throw Exception("flag \"" + name + "\" is not added globally");
  }

  return flag;
}

std::tuple<const flags::Flag&, bool> Context::TryToGetGlobalFlag(
    const std::string& name) const {
  if (parent == nullptr) {
    return {flags::Flag(), false};
  }

  try {
    auto& flag = parent->Flag(name);
    return {flag, true};
  } catch (const Exception&) {
    return parent->TryToGetGlobalFlag(name);
  }
}

const typename Context::parent_t& Context::Parent() const noexcept {
  return parent;
}

const flags::Flag& Context::Flag(const std::string& name) const {
  try {
    return flags.GetFlag(name);
  } catch (const flags::Exception& e) {
    throw Exception(e.What());
  }
}

const std::vector<std::string>& Context::Args() const noexcept {
  return flags.Args();
}
}  // namespace pineapple