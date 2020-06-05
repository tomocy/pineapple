#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "external/flags/src/flags/flags.h"

namespace pineapple {
class Context {
 public:
  using parent_t = std::unique_ptr<Context>;

  static parent_t Make(Context&& ctx) noexcept;

  Context(const flags::FlagSet& flags) noexcept;

  Context(parent_t&& parent, const flags::FlagSet& flags) noexcept;

  std::tuple<const flags::Flag&, bool> TryToGetGlobalFlag(const std::string& name) const;

  const parent_t& Parent() const noexcept;

  const flags::Flag& Flag(const std::string& name) const;

  const std::vector<std::string>& Args() const noexcept;

 private:
  parent_t parent;
  const flags::FlagSet& flags;
};
}  // namespace pineapple

#endif