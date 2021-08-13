#include "video.h"

#include <iostream>
#include <utility>
#include <vector>

Video::Video(std::string&& title, std::string&& videoId,
             std::vector<std::string>&& tags) :
  mTitle(std::move(title)),
  mVideoId(std::move(videoId)),
  mTags(std::move(tags)) {
}

const std::string& Video::getTitle() const { return mTitle; }

const std::string& Video::getVideoId() const { return mVideoId; }

const std::vector<std::string>& Video::getTags() const { return mTags; }

void Video::printVideo() const {
    std::cout << mTitle << " (" << mVideoId << ") [";
    if (mTags.size() == 0) { std::cout << "] " << std::endl; }
    else {
        for (int j = 0; j < mTags.size() - 1; j++) {
            std::cout << mTags[j] << " ";
        }
        std::cout << mTags[mTags.size() - 1] << "]";
    }
}

void Video::flag(const std::string& reason) {
    flagged = true;
    flagReason = reason;
}

void Video::flag() {
    flagged = true;
    flagReason = "Not supplied";
}

std::string Video::getFlagReason() const {
    return flagReason;
}

bool Video::isFlagged() const {
    return flagged;
}

void Video::removeFlag() {
    flagged = false;
    flagReason.clear();
}
