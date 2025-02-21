#ifndef YTDLP_HANLDER_HXX
#define YTDLP_HANLDER_HXX

#include "queue_manager.hxx"
#include <string>

class YtdlpHandler {
  public:
    Link getMp3Url(const Link& url);
  private:
    std::string executeCommand(const char* cmd);
};

#endif
