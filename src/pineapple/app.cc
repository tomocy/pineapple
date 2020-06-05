#include "src/pineapple/app.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/command.h"
#include "src/pineapple/context.h"
#include "src/pineapple/exceptions.h"

namespace pineapple {
App::App(const std::string& name) : Command(name) {}

App::App(const std::string& name, const std::string& description)
    : Command(name, description, nullptr) {}

App::App(const std::string& name, const action_t& action)
    : Command(name, action) {}

App::App(const std::string& name, const std::string& description,
         const Command::action_t& action)
    : Command(name, description, action) {}

void App::Run(const std::vector<std::string>& args) {
  if (args.size() < 1) {
    throw Exception(
        "insufficient arguments: one argument is required at least");
  }

  auto trimmed = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  flags.Parse(trimmed);

  auto ctx = Context(flags);

  if (ctx.Args().size() >= 1 && DoHaveCommand(ctx.Args().at(0))) {
    RunCommand(std::move(ctx));
    return;
  }

  if (ctx.Args().empty() || action != nullptr) {
    DoAction(ctx);
    return;
  }

  throw Exception("argument\"" + ctx.Args().at(0) +
                  "\" is not handled at all: action or command named \"" +
                  ctx.Args().at(0) + "\" is needed");
}
}  // namespace pineapple