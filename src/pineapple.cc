#include "src/pineapple.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace pineapple {
Command::Command(const std::string& name) noexcept : name(name) {}

std::string Command::Help() const noexcept { return name; }
}  // namespace pineapple

// namespace pineapple {
// Flag::Flag(const std::string& name) noexcept : Flag(name, "") {}

// Flag::Flag(const std::string& name, const std::string& value) noexcept
//     : name(name), value(value) {}

// const std::string& Flag::Name() const noexcept { return name; }

// const std::string& Flag::Value() const noexcept { return value; }
// }  // namespace pineapple

// namespace pineapple {
// Context::Context(const std::map<std::string, Flag>& flags,
//                  const std::vector<std::string>& args) noexcept
//     : flags(flags), args(args) {}

// std::string Context::Get(const std::string& name) const noexcept {
//   auto [flag, found] = Find(name);
//   if (!found) {
//     return "";
//   }

//   return flag.Value();
// }

// const std::vector<std::string>& Context::Args() const noexcept { return args;
// }

// std::tuple<Flag, bool> Context::Find(const std::string& name) const noexcept
// {
//   if (flags.find(name) == flags.end()) {
//     return {Flag(), false};
//   }

//   return {flags.at(name), true};
// }
// }  // namespace pineapple

// namespace pineapple {
// Context Parser::Parse(const std::vector<std::string>& src) const noexcept {
//   std::cout << "Parser::Parse" << std::endl;
//   auto flags = std::map<std::string, Flag>();
//   auto args = std::vector<std::string>();

//   if (src.empty()) {
//     return Context(flags, args);
//   }
//   if (src.size() == 1) {
//     args.push_back(src.at(0));
//     return Context(flags, args);
//   }

//   for (auto first = std::begin(src), second = std::begin(src) + 1;
//        first != std::end(src); first += 2, second += 2) {
//     if (second == std::end(src) || !(IsFlagName(*first))) {
//       std::for_each(first, std::end(src),
//                     [&args](auto arg) { args.push_back(arg); });
//       break;
//     }

//     auto name = TrimFlagName(*first);
//     auto value = *second;
//     flags[name] = Flag(name, value);
//   }

//   return Context(flags, args);
// }  // namespace pineapple

// bool Parser::IsFlagName(const std::string& name) const noexcept {
//   auto i = name.find("--");
//   return i != std::string::npos && i == 0;
// }

// std::string Parser::TrimFlagName(const std::string& name) const noexcept {
//   auto i = name.find_first_not_of("-");
//   if (i == std::string::npos) {
//     return name;
//   }

//   return name.substr(i);
// }
// }  // namespace pineapple

// namespace pineapple {
// Command::Command(const std::string& name) noexcept : name(name) {}

// const std::string& Command::Name() const noexcept { return name; }

// Command& Command::WithAction(
//     const typename Command::action_t& action) noexcept {
//   this->action = action;
//   return *this;
// }

// void Command::Run(const std::vector<std::string>& args) const noexcept {
//   auto parser = Parser();
//   auto ctx = parser.Parse(args);
//   action(ctx);
// }
// }  // namespace pineapple

// namespace pineapple {
// void App::AddCommand(const ::pineapple::Command& cmd) noexcept {
//   commands[cmd.Name()] = cmd;
// }

// void App::Run(const std::vector<std::string>& args) const noexcept {
//   if (args.size() < 2) {
//     return;
//   }

//   auto name = args.at(1);
//   auto [cmd, found] = FindCommand(name);
//   if (!found) {
//     // TODO: show help
//     return;
//   }

//   auto raw_args = (args.size() >= 3) ? std::vector<std::string>(
//                                            std::begin(args) + 2,
//                                            std::end(args))
//                                      : std::vector<std::string>();
//   cmd.Run(raw_args);
// }

// std::tuple<Command, bool> App::FindCommand(const std::string& name) const
//     noexcept {
//   if (commands.find(name) == commands.end()) {
//     return {Command(), false};
//   }

//   return {commands.at(name), true};
// }
// }  // namespace pineapple