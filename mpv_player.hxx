#ifndef MPV_PLAYER_HXX
#define MPV_PLAYER_HXX

#include <condition_variable>
#include <mpv/client.h>
#include <thread>
#include "queue_manager.hxx"

class MpvPlayer
{
  public:
    MpvPlayer();
    ~MpvPlayer();
    void play(const Link& lnk);
    void stop();
    void pause();
    bool isFinished();
    void setVolume(double volume);
    void waitUntilFinished();
  private:
    void checkError(int status);
    void musicLoop();

    mpv_handle* mpv = nullptr;
    std::thread musicThread;
    std::atomic<bool> running{false};
    std::atomic<bool> finished;
    std::atomic<int> paused;
    std::mutex mtx;  // Для блокировки
    std::condition_variable cv;  // Условная переменная
};

#endif
