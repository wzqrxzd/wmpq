#include <spdlog/spdlog.h>
#include "mpv_player.hxx"
#include "queue_manager.hxx"
#include "ytdlp_handler.hxx"
#include <chrono>

int main()
{
  Link lnk("https://music.youtube.com/playlist?list=OLAK5uy_lcDeApsFqCVxlrC3_4P3HDfFV0uAsbFZs&si=esI9bGbR_ely1123");
  Link lnk2("https://www.youtube.com/watch?v=EkRljQk0Gxw");
  QueueManager queue;
  YtdlpHandler yt;
  MpvPlayer mpv;

  queue.addLink(lnk);
  queue.addLink(lnk2);

  while (!queue.isEmpty())
  {
    Link lnkTrack = queue.getLink();
    Link mp3Url = yt.getMp3Url(lnkTrack);

    mpv.play(mp3Url);
    mpv.setVolume(40);
    mpv.waitUntilFinished();
  }

  /*while (!mpv.isFinished()) {*/
  /*  std::this_thread::sleep_for(std::chrono::milliseconds(100));*/
  /*}*/
}

