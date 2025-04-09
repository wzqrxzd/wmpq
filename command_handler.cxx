#include "command_handler.hxx"
#include <spdlog/spdlog.h>
#include <sstream>

void CommandHandler::registerCommand(const std::string& name, HandlerFunc handler)
{
  commandMap[name] = handler;
}

void CommandHandler::handleCommand(const std::string& input)
{
  auto [cmd, args] = parseCommand(input);
  auto it = commandMap.find(cmd);
  if (it!=commandMap.end())
    it->second(args);
  else
    spdlog::error("Unknown command {}.", cmd);
}

std::pair<std::string, std::string> CommandHandler::parseCommand(const std::string& input)
{
  std::istringstream iss(input);
  std::string cmd;
  iss >> cmd;
  std::string args;
  std::getline(iss, args);
  if (!args.empty() && args[0] == ' ')
    args.erase(0, 1);
  return {cmd, args};
}
