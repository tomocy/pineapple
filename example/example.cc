#include <string>
#include <vector>

#include "src/pineapple.h"

int main() {
  auto args = std::vector<std::string>();

  auto app = pineapple::Command("example", "an example cli app");
  app.AddCommand(pineapple::Command("sub-A", "a subcommand to do A"));
  app.Run(args);

  return 0;
}