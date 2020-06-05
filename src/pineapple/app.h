#ifndef TOMOCY_PINEAPPLE_PINEAPPLE_APP_H
#define TOMOCY_PINEAPPLE_PINEAPPLE_APP_H

#include <functional>
#include <string>
#include <vector>

#include "external/flags/src/flags/flags.h"
#include "src/pineapple/command.h"
#include "src/pineapple/context.h"

namespace pineapple {
class App : public Command {
 public:
  App(const std::string& name);

  App(const std::string& name, const std::string& description);

  App(const std::string& name, const std::string& description,
      const action_t& action);

  void Run(const std::vector<std::string>& args);
};
}  // namespace pineapple

#endif