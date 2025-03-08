#include "cli.hxx"
#include <iostream>

void Cli::eventLoop()
{
  spdlog::info("Commands: p <link> (play), s (stop), v <0-100> (volume), q (quit)");

  std::string command;

  setVolume(40);

  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, command);

    if (command == "q")
    {
      quit();
    }
    else if (command == "s")
    {
      stop();
    } else if (command == "sk")
    {
      skip();
    } else if (command == "l")
    {
      loop();
    } else if (command == "ps") {
      pause();
    }
    else if (command.starts_with("p "))
    {
      std::string lnkStr = command.substr(2);
      Link lnk(lnkStr);
      try {
        Link mp3Url = YtdlpHandler::getMp3Url(lnk);
        queue.addLink(mp3Url);
      } catch (const std::runtime_error& e)
      {
        spdlog::error("Link is not valid.");
        continue;
      }
      
      if (!playing)
      {
        playing = true;
        if (playThread.joinable())
          playThread.join();
        playThread = std::thread(&Cli::play, this);
      }
    } else if (command.starts_with("v "))
    {
      int volume = std::stoi(command.substr(2));
      setVolume(volume);
    } else {
      spdlog::error("Unknown command.");
    }
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

void Cli::setVolume(int volume)
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
