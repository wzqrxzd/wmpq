#ifndef CLI_HXX
#define CLI_HXX

#include <spdlog/spdlog.h>
#include "mpv_player.hxx"
#include "queue_manager.hxx"
#include "ytdlp_handler.hxx"
#include "command_handler.hxx"

class Cli {
  public:
    void eventLoop();
  private:
    MpvPlayer mpv;
    QueueManager queue;
    CommandHandler handler;

    std::atomic<bool> playing{false};
    std::thread playThread;

    void registerCommands();

    void play();
    void stop();
    void pause();
    void quit();
    void setVolume(int volume);
    void loop();
    void skip();
    void playCommand(const std::string& args);
    void playlistCommand(const std::string& args);
};

#endif
