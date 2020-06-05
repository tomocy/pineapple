#include <iostream>
#include <string>
#include <vector>

#include "src/pineapple/pineapple.h"

int main(int n, const char** args) {
  // App with its name and description
  auto app = pineapple::App("app", "an example cli app");

  auto user_cmd = pineapple::Command("user");

  // Command named "create" with an action
  auto create_user_cmd = pineapple::Command(
      "create", "create a user",
      [](pineapple::Command::const_action_ctx_t ctx) {
        // "ctx" holds the context.
        // You can get the flags and arguments through this context.
        auto name = ctx.Flag("name").Get<std::string>();
        std::cout << "User \"" + name + "\" is successfully created."
                  << std::endl;
      });

  // You can add flag to a command.
  // std::string, int, bool can be used as the value of a flag.
  create_user_cmd.AddFlag(flags::Flag("name", flags::String::Make("")));

  // The "create" command is added to "user" command.
  // The action of "create" command is invoked by "user create"
  // The "name" flag of "create" command is provided with "user create --name"
  user_cmd.AddCommand(std::move(create_user_cmd));

  // The "user" command is registred in the "app" app.
  app.AddCommand(std::move(user_cmd));

  // pineapple::Exception is thrown when an app fails to run.
  try {
    app.Run(n, args);
  } catch (const pineapple::Exception& e) {
    // App can generate its usage based on the added flags and commands.
    std::cerr << app.Usage() << std::endl;
    std::cerr << e.What() << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}