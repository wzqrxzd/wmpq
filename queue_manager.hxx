#ifndef QUEUE_MANAGER_HXX
#define QUEUE_MANAGER_HXX

#include <deque>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct Link {
  bool isValid() const;
  std::string url;
};

class QueueManager
{
  public:
    QueueManager();
    bool isEmpty();
    void addLink(const Link& lnk);
    void setPlaylist(const fs::path& playlistPath);
    Link getLink();
    void clear();
    bool loop();
    void printDeque();
  private:
    std::deque<Link> musicDeque;
    bool looped{false};
};

#endif
