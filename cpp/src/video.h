#pragma once

#include <string>
#include <vector>

/**
 * A class used to represent a video.
 */
class Video {
 private:
  std::string mTitle;
  std::string mVideoId;
  std::vector<std::string> mTags;
  bool isPlaying = 0;
  bool flagged = false;
  std::string flagReason;

 public:
  Video(std::string&& title, std::string&& videoId,
        std::vector<std::string>&& tags);

  // Returns the title of the video.
  const std::string& getTitle() const;

  // Returns the video id of the video.
  const std::string& getVideoId() const;

  // Returns a readonly collection of the tags of the video.
  const std::vector<std::string>& getTags() const;

  void printVideo() const;

  void flag(const std::string& reason);
  void flag();
  std::string getFlagReason() const;
  bool isFlagged() const;
  void removeFlag();
};
