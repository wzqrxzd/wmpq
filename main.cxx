#include <spdlog/spdlog.h>
#include "mpv_player.hxx"
#include "queue_manager.hxx"
#include "ytdlp_handler.hxx"
#include "cli.hxx"

int main()
{
  Cli cli;
  cli.eventLoop();
}
