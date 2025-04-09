#include "queue_manager.hxx"
#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <fstream>
#include "ytdlp_handler.hxx"

bool Link::isValid() const
{
  CURL* curl = curl_easy_init();
  if (!curl)
    throw std::runtime_error("Curl initialization error.");

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // Не загружаем содержимое, только проверяем заголовки
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Разрешаем редиректы

  CURLcode res = curl_easy_perform(curl);
  long response_code = 0;

  if (res == CURLE_OK) {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  }

  curl_easy_cleanup(curl);
    
  return (res == CURLE_OK && response_code == 200);
}

QueueManager::QueueManager() : musicDeque() {};

void QueueManager::addLink(const Link& lnk)
{
  musicDeque.push_back(lnk);
}

bool QueueManager::isEmpty()
{
  return musicDeque.empty();
}

Link QueueManager::getLink()
{
  Link tmp = musicDeque.front();
  musicDeque.pop_front();
  if (looped)
    musicDeque.push_back(tmp);
  return tmp;
}

bool QueueManager::loop()
{
  looped = !looped;
  spdlog::info({"Looped: {}"}, looped);
  return looped;
}

void QueueManager::printDeque() {
  if (musicDeque.empty()) {
    spdlog::info("The queue is empty.");
    return;
  }

  spdlog::info("Queue contents:");
  for (const auto& link : musicDeque) {
    spdlog::info("URL: {}", link.url);
  }
}

void QueueManager::clear()
{
  musicDeque.clear();
}

void QueueManager::setPlaylist(const fs::path& playlistPath)
{
  clear();

  if (!fs::exists(playlistPath))
    spdlog::warn("Playlist({}) not exist.", playlistPath.string());

  std::ifstream playlistFile(playlistPath);
  std::string track;
  
  while (std::getline(playlistFile, track))
  {
    spdlog::info("{}", track);
    auto lnk = YtdlpHandler::getMp3Url(Link(track));
    addLink(Link(lnk));
  }

  playlistFile.close();
}
