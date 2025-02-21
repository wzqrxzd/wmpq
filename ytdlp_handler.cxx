#include "ytdlp_handler.hxx"
#include <format>
#include <cstdio>
#include <array>
#include <memory>

std::string YtdlpHandler::executeCommand(const char* cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

  if (!pipe)
    throw std::runtime_error("Error during popen call.");

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    result += buffer.data();

  return result;
}

Link YtdlpHandler::getMp3Url(const Link& url)
{
  if (!url.isValid())
    throw std::runtime_error("Link is not valid.");
  std::string command = std::format("yt-dlp -f bestaudio --get-url \'{}\'", url.url);
  return Link(executeCommand(command.c_str()));
}
