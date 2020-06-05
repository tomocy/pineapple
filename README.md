# pineapple

![Test](https://github.com/tomocy/pineapple/workflows/Test/badge.svg?branch=master)

A library to build command-line applications.

## Example

```cpp
#include <iostream>
#include <string>
#include <vector>

#include "external/pineapple/src/pineapple/pineapple.h"

int main(int n, const char** args) {
  auto app = pineapple::App("app", "an example cli app");

  auto user_cmd = pineapple::Command("user");

  auto create_user_cmd = pineapple::Command(
      "create", "create a user",
      [](pineapple::Command::const_action_ctx_t ctx) {
        auto name = ctx.Flag("name").Get<std::string>();
        std::cout << "User \"" + name + "\" is successfully created." << std::endl;
      });

  create_user_cmd.AddFlag(flags::Flag("name", flags::String::Make("")));

  user_cmd.AddCommand(std::move(create_user_cmd));

  app.AddCommand(std::move(user_cmd));

  try {
    app.Run(n, args);
  } catch (const pineapple::Exception& e) {
    std::cerr << app.Usage() << std::endl;
    std::cerr << e.What() << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
```

If you provided the following arguments to the code above,

```
./app user create --name alice
```

The output should be below.

```
User "alice" is successfully created.
```

You can see this example with description in [example.cc](example/example.cc).

## Requirements

- [Bazel](https://bazel.build/)
- a C++17 compliant compiler

## Usage

Include `pineapple.h` as below.

```cpp
#include "external/pineapple/src/pineapple/pineapple.h"
```

## Contribution

1. Fork
2. Create a branch
3. Commit your changes
4. Rebase the branch of yours against the master branch of this repository
5. Run `bazel tests //...` to test
6. Create a pull request for the changes of yours

## Licenses
- [MIT License](LICENSE)
