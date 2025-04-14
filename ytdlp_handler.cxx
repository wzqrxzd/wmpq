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

std::pair<std::string, std::string> YtdlpHandler::getInfo(const Link& url)
{
  if (!url.isValid())
    throw std::runtime_error("Link is not valid.");

  std::string command = std::format(
    "yt-dlp --print \"%(title)s|%(uploader)s\" '{}'", url.url
  );
  std::string output = executeCommand(command.c_str());

  // Remove trailing newlines (optional)
  output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());

  // Split the output into title and uploader
  size_t delimiterPos = output.find('|');
  if (delimiterPos == std::string::npos)
    throw std::runtime_error("Unexpected output format from yt-dlp.");

  std::string title = output.substr(0, delimiterPos);
  std::string uploader = output.substr(delimiterPos + 1);

  return { title, uploader };
}
