#include "cli.hxx"
#include <iostream>

void Cli::eventLoop()
{
  spdlog::info("Commands: p <link> (play), s (stop), q (quit)");

  std::string command;

  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "q")
    {
      stop();
      if (playThread.joinable())
        playThread.join();
      spdlog::info("exited.");
    }
    else if (command == "s")
    {
      stop();
      spdlog::info("Playback stopped.");
    }
    else if (command.rfind("p"))
    {
      std::string lnk = command.substr(2);
      queue.addLink(Link(lnk));
      
      if (!playing)
      {
        playing = true;
        if (playThread.joinable())
          playThread.join();
        playThread = std::thread(&Cli::play, this);
      }
    }
  }
  if (playThread.joinable())
    playThread.join();
}

void Cli::quit() {
  stop();
  if (playThread.joinable())
    playThread.join();
  spdlog::info("exited.");
}

void Cli::stop()
{
  stop();
  playing = false;
  spdlog::info("Playback stoped.");
}

void Cli::play()
{
  while (playing)
  {
    if (!queue.isEmpty())
    {
      Link lnkTrack = queue.getLink();
      Link mp3Url = YtdlpHandler::getMp3Url(lnkTrack);

      mpv.play(mp3Url);
      mpv.setVolume(40);

      while (playing && !mpv.isFinished())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    } else {
      playing = false;
    }
  }
}
