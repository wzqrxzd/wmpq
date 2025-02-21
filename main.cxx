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
  Link lnkTrack1 = queue.getLink();
  Link lnkTrack2 = queue.getLink();

  auto str = yt.getMp3Url(lnkTrack1);
  auto str2 = yt.getMp3Url(lnkTrack2);

  mpv.play(str);
  mpv.setVolume(40);
  mpv.waitUntilFinished();
  /*while (!mpv.isFinished()) {*/
  /*  std::this_thread::sleep_for(std::chrono::milliseconds(100));*/
  /*}*/

  mpv.play(str2);
  mpv.setVolume(20);
  mpv.waitUntilFinished();
  /*while (!mpv.isFinished()) {*/
  /*  std::this_thread::sleep_for(std::chrono::milliseconds(100));*/
  /*}*/
}

