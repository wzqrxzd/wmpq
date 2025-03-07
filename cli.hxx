#ifndef CLI_HXX
#define CLI_HXX

#include <spdlog/spdlog.h>
#include "mpv_player.hxx"
#include "queue_manager.hxx"
#include "ytdlp_handler.hxx"

class Cli {
  public:
    void eventLoop();
  private:
    MpvPlayer mpv;
    QueueManager queue;
    std::atomic<bool> playing{false};
    std::thread playThread;

    void play();
    void stop();
    void quit();
};

#endif
