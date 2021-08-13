#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include "videolibrary.h"

/** A class used to represent a Playlist */

class VideoPlaylist {
private:
	std::vector<std::string> mVideos;

public:
	VideoPlaylist() = default;

	// This class is not copyable to avoid expensive copies.
	VideoPlaylist(const VideoPlaylist&) = delete;
	VideoPlaylist& operator=(const VideoPlaylist&) = delete;

	// This class is movable.
	VideoPlaylist(VideoPlaylist&&) = default;
	VideoPlaylist& operator=(VideoPlaylist&&) = default;

	bool addVideo(const std::string& newVideoId);
	std::vector<std::string> getVideos() const;
	bool removeVideo(const std::string& videoId);
	void clearVideos();
};
