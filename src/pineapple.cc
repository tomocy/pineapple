#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "src/flag_lexer.h"

namespace pineapple {
Flag::Flag(const std::string& name, const std::string& value) noexcept
    : name(name), value(value) {}

const std::string& Flag::Name() const noexcept { return name; }

const std::string& Flag::Value() const noexcept { return value; }
}  // namespace pineapple

namespace pineapple {
Parser::Parser(const Lexer& lexer) noexcept
    : lexer(lexer),
      currToken(kTokenEOF),
      nextToken(kTokenEOF),
      flags(std::vector<Flag>()),
      args(std::vector<std::string>()) {
  ReadToken();
  ReadToken();
}

void Parser::Parse() noexcept {
  while (true) {
    switch (currToken.Kind()) {
      case TokenKind::END_OF_FILE:
      case TokenKind::UNKNOWN:
        return;
      case TokenKind::SHORT_HYPHEN:
      case TokenKind::LONG_HYPHEN:
        ParseFlag();
        break;
      case TokenKind::STRING:
        ParseArg();
        break;
      default:
        ReadToken();
    }
  }
}

const std::vector<Flag>& Parser::Flags() const noexcept { return flags; }

const std::vector<std::string>& Parser::Args() const noexcept { return args; }

void Parser::ParseFlag() noexcept {
  ReadToken();

  auto name = ParseString();

  if (DoHave(TokenKind::EQUAL)) {
    ReadToken();
  }

  if (DoHave(TokenKind::SHORT_HYPHEN) || DoHave(TokenKind::LONG_HYPHEN)) {
    flags.push_back(Flag(name, ""));
    return;
  }

  auto value = ParseString();
  flags.push_back(Flag(name, value));
}

void Parser::ParseArg() noexcept {
  auto arg = ParseString();
  args.push_back(arg);
}

std::string Parser::ParseString() noexcept {
  auto literal = currToken.Literal();

  ReadToken();

  return literal;
}

bool Parser::DoHave(TokenKind kind) const noexcept {
  return currToken.Kind() == kind;
}

void Parser::ReadToken() noexcept {
  currToken = nextToken;
  nextToken = lexer.ReadToken();
}
}  // namespace pineapple

namespace pineapple {
FlagSet::FlagSet(const std::vector<Flag>& flags) noexcept : flags(flags) {}

void FlagSet::Parse(const std::vector<std::string>& args) noexcept {
  // auto src = Source(args);
  // auto lexer = Lexer(src);
  // auto parser = Parser(lexer);

  // parser.Parse();

  // flags = parser.Flags();
  // args = parser.Args();
}

const std::vector<std::string>& FlagSet::Args() const noexcept { return args; }

std::vector<char> FlagSet::Source(const std::vector<std::string>& args) const
    noexcept {
  std::ostringstream src_builder;
  std::copy(std::begin(args), std::end(args),
            std::ostream_iterator<std::string>(src_builder, " "));

  auto src = src_builder.str();

  return std::vector<char>(std::begin(src), std::end(src));

  return std::vector<char>(std::begin(src), std::end(src));
}
}  // namespace pineapple

namespace pineapple {
Command::Command(const std::string& name,
                 const std::string& description) noexcept
    : Command(name, description, [this](auto _) { this->PrintHelp(); }) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action) noexcept
    : Command(name, description, action, std::vector<Command>()) {}

Command::Command(const std::string& name, const std::string& description,
                 const typename Command::action_t& action,
                 const std::vector<Command>& commands) noexcept
    : name(name),
      description(description),
      action(action),
      commands(commands) {}

void Command::Run(const std::vector<std::string>& args) const noexcept {
  if (args.size() < 1) {
    return;
  }

  if (commands.size() >= 1 && args.size() >= 2) {
    auto name = args.at(1);
    auto sliced =
        std::vector<std::string>(std::begin(args) + 2, std::end(args));
    RunAsCommand(name, sliced);

    return;
  }

  auto sliced = std::vector<std::string>(std::begin(args) + 1, std::end(args));
  action(sliced);
}

void Command::AddCommand(const Command& cmd) noexcept {
  commands.push_back(cmd);
}

void Command::PrintHelp() const noexcept { std::cout << Help() << std::endl; }

std::string Command::Help() const noexcept {
  auto help = name + " - " + description;
  auto cmdsHelp = CommandsHelp();
  if (!cmdsHelp.empty()) {
    help += "\n\n" + cmdsHelp;
  }

  return help;
}

void Command::RunAsCommand(const std::string& name,
                           const std::vector<std::string>& args) const
    noexcept {
  auto [cmd, found] = FindCommand(name);
  if (!found) {
    PrintHelp();
    return;
  }

  cmd.Run(args);
}

std::tuple<Command, bool> Command::FindCommand(const std::string& name) const
    noexcept {
  for (auto cmd : commands) {
    if (cmd.name == name) {
      return {cmd, true};
    }
  }

  return {Command(name, ""), false};
}

std::string Command::CommandsHelp() const noexcept {
  if (commands.size() == 0) {
    return "";
  }

  auto help = std::string("Commands:\n");

  for (auto cmd : commands) {
    help += cmd.name + "    " + cmd.description + "\n";
  }

  return help.erase(help.size() - 1, 1);
}
}  // namespace pineapple