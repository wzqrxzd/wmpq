#include "cli.hxx"
#include "command_handler.hxx"
#include <iostream>

void Cli::registerCommands()
{
  handler.registerCommand("q", [this](const std::string&){
      quit();
  });

  handler.registerCommand("s", [this](const std::string&){
      stop();
  });

  handler.registerCommand("sk", [this](const std::string&){
      skip();
  });

  handler.registerCommand("l", [this](const std::string&){
      loop();
  });

  handler.registerCommand("ps", [this](const std::string&){
      pause();
  });

  handler.registerCommand("p", [this](const std::string& args){
      playCommand(args);
  });

  handler.registerCommand("playlist", [this](const std::string& args){
      playlistCommand(args);
  });

  handler.registerCommand("v", [this](const std::string& args){
      try {
        int volume = std::stoi(args);
        this->volume(std::stoi(args));
      } catch (...)
      {
        spdlog::error("Invalid volume.");
      }
  });

}

void Cli::eventLoop()
{
  registerCommands();

  spdlog::info("Commands: p <link> (play), s (stop), v <0-100> (volume), sk (skip), l (loop), ps (pause), q (quit)");

  volume(40);
  
  std::string command;
  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, command);
    handler.handleCommand(command);
  }

  if (playThread.joinable())
    playThread.join();
}

void Cli::quit() {
  mpv.stop();
  playing = false;

  if (playThread.joinable())
    playThread.join();
  exit(0);
}

void Cli::stop()
{
  mpv.stop();
  queue.clear();
  playing = false;
  spdlog::info("Playback stopped.");
}

void Cli::playCommand(const std::string& args)
{
  Link lnk(args);
  try {
    Link mp3Url = YtdlpHandler::getMp3Url(lnk);
    queue.addLink(mp3Url);
  } catch (const std::runtime_error& e)
  {
    spdlog::error("Link is not valid.");
    return;
  }

  if (!playing)
  {
    playing = true;
    if (playThread.joinable())
      playThread.join();
    playThread = std::thread(&Cli::play, this);
  }     
}

void Cli::play()
{
  while (playing)
  {
    if (!queue.isEmpty())
    {
      Link lnkTrack = queue.getLink();

      mpv.play(lnkTrack);

      while (playing && !mpv.isFinished())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    } else {
      playing = false;
    }
  }
}

void Cli::volume(int volume)
{
  mpv.setVolume(volume);
  spdlog::info("Volume setted to {}%", volume);
}

void Cli::loop()
{
  queue.loop();
}

void Cli::skip()
{
  mpv.stop();
  playing = false;

  spdlog::info("Skipping track...");

  if (!queue.isEmpty())
  {
    spdlog::debug("Queue is not empty, starting next track.");

    if (playThread.joinable())
      playThread.join();

    playing = true;

    playThread = std::thread(&Cli::play, this);
  }
  else
  {
    spdlog::info("Queue is empty, nothing to skip to.");
  }
}

void Cli::pause()
{
  mpv.pause();
  playing = !playing;
}

void Cli::playlistCommand(const std::string& args)
{
  stop();
  queue.setPlaylist(args);
  if (!playing)
  {
    playing = true;
    if (playThread.joinable())
      playThread.join();
    playThread = std::thread(&Cli::play, this);
  }
}
