#include "mpv_player.hxx"
#include <mpv/client.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <format>

void MpvPlayer::waitUntilFinished()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return finished.load(); });  // Ждем, пока музыка не закончится
}

bool MpvPlayer::isFinished()
{
  return finished;
}

void MpvPlayer::checkError(int status)
{
  if (status < 0)
    throw std::runtime_error(std::format("MPV Error: {}.", mpv_error_string(status)));
}

MpvPlayer::MpvPlayer()
{
  mpv = mpv_create();
  if (!mpv)
    throw std::runtime_error("Error during creating MPV context.");

  checkError(mpv_initialize(mpv));

  running = true;
  musicThread = std::thread(&MpvPlayer::musicLoop, this);
}

MpvPlayer::~MpvPlayer()
{
  running = false;
  if (musicThread.joinable())
    musicThread.join();

  if (mpv)
    mpv_terminate_destroy(mpv);
}

void MpvPlayer::play(const Link& lnk)
{
  finished = false;
  const char* cmd[] = {"loadfile", lnk.url.c_str(), NULL};
  checkError(mpv_command(mpv, cmd));
  spdlog::debug("Playing: {}", lnk.url);
}

void MpvPlayer::stop()
{
  const char* cmd[] = {"stop", NULL};
  checkError(mpv_command(mpv, cmd));
  finished = true;
  cv.notify_all();
  spdlog::debug("Playback stopped.");
}

void MpvPlayer::pause()
{
  paused = !paused;
  checkError(mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &paused));
  spdlog::info(paused ? "Paused." : "Resumed.");
}

void MpvPlayer::setVolume(double volume)
{
  volume = std::clamp(volume, 0.0, 100.0);
  checkError(mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &volume));
  spdlog::debug("Volume set to {}%", volume);
}

void MpvPlayer::musicLoop()
{
  while (running)
  {
    mpv_event* event = mpv_wait_event(mpv, 100);
    if (event->event_id == MPV_EVENT_END_FILE)
    {
      spdlog::debug("Playback ended.");
      finished = true;
      cv.notify_all();
    }
  }
}
