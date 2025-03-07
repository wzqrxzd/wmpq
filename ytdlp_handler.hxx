#ifndef YTDLP_HANLDER_HXX
#define YTDLP_HANLDER_HXX

#include "queue_manager.hxx"
#include <string>

class YtdlpHandler {
  public:
    static Link getMp3Url(const Link& url);
  private:
    static std::string executeCommand(const char* cmd);
};

#endif
