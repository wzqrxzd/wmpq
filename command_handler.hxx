#ifndef COMMAND_INTERPRITATOR_HXX
#define COMMAND_INTERPRITATOR_HXX

#include <functional>
#include <unordered_map>
#include <string>

class CommandHandler {
  public:
    using HandlerFunc = std::function<void(const std::string&)>;
    void registerCommand(const std::string& name, HandlerFunc handler);
    void handleCommand(const std::string& input);
  private:
    std::unordered_map<std::string, HandlerFunc> commandMap;
    std::pair<std::string, std::string> parseCommand(const std::string& input);
};

#endif
