#include "videoplaylist.h"

bool VideoPlaylist::addVideo(const std::string& newVideoId) {
	for (std::string Id : mVideos) {
		if (Id == newVideoId) {
			return false;
		}
	}
	mVideos.emplace_back(newVideoId);
	return true;
}

std::vector<std::string> VideoPlaylist::getVideos() const {
	return mVideos;
}

bool VideoPlaylist::removeVideo(const std::string& videoId) {
	for (int i = 0; i < mVideos.size(); i++) {
		if (mVideos[i] == videoId) {
			mVideos.erase(mVideos.begin() + i);
			return true;
		}
	}
	return false;
}

void VideoPlaylist::clearVideos() {
	mVideos.clear();
}
