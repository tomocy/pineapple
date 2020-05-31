#ifndef TOMOCY_PINEAPPLE_H
#define TOMOCY_PINEAPPLE_H

#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include "src/flag_parser.h"

namespace pineapple {
class Flag {
 public:
  Flag(const std::string& name, const std::string& value) noexcept;

  const std::string& Name() const noexcept;

  const std::string& Value() const noexcept;

 private:
  std::string name;
  std::string value;
};
}  // namespace pineapple

namespace pineapple {
class Parser {
 public:
  Parser(const Lexer& lexer) noexcept;

  void Parse() noexcept;

  const std::vector<Flag>& Flags() const noexcept;

  const std::vector<std::string>& Args() const noexcept;

 private:
  void ParseFlag() noexcept;

  void ParseArg() noexcept;

  std::string ParseString() noexcept;

  bool DoHave(TokenKind kind) const noexcept;

  void ReadToken() noexcept;

  Lexer lexer;
  Token currToken;
  Token nextToken;

  std::vector<Flag> flags;
  std::vector<std::string> args;
};
}  // namespace pineapple

namespace pineapple {
class FlagSet {
 public:
  FlagSet(const std::vector<Flag>& flags) noexcept;

  void Parse(const std::vector<std::string>& args) noexcept;

  const std::vector<std::string>& Args() const noexcept;

 private:
  std::vector<char> Source(const std::vector<std::string>& args) const noexcept;

  std::vector<Flag> flags;
  std::vector<std::string> args;
};
}  // namespace pineapple

namespace pineapple {
class Command {
 public:
  using action_t = std::function<void(const std::vector<std::string>& args)>;

  Command(const std::string& name, const std::string& description) noexcept;

  Command(const std::string& name, const std::string& description,
          const action_t& action) noexcept;

  void Run(const std::vector<std::string>& args) const noexcept;

  void AddCommand(const Command& cmd) noexcept;

  void PrintHelp() const noexcept;

  std::string Help() const noexcept;

 private:
  Command(const std::string& name, const std::string& description,
          const action_t& action,
          const std::vector<Command>& commands) noexcept;

  void RunAsCommand(const std::string& name,
                    const std::vector<std::string>& args) const noexcept;

  std::tuple<Command, bool> FindCommand(const std::string& name) const noexcept;

  std::string CommandsHelp() const noexcept;

  std::string name;
  std::string description;
  action_t action;
  std::vector<Command> commands;
};
}  // namespace pineapple

#endif