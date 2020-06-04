#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_CONTEXT_H

#include "external/flags/src/flags/flags.h"

namespace pineapple {
class Context {
 public:
  Context(flags::FlagSet&& flags) noexcept;

 private:
  flags::FlagSet flags;
};
}  // namespace pineapple

#endif