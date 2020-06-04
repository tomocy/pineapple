#include "src/pineapple/context.h"

#include "external/flags/src/flags/flags.h"

namespace pineapple {
Context::Context(flags::FlagSet&& flags) noexcept : flags(std::move(flags)) {}
}