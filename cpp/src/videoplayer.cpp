#include "videoplayer.h"

#include <iostream>
#include <algorithm>

void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
    std::vector<Video> videos = mVideoLibrary.getVideos();
    std::cout << "Here's a list of all available videos: " << std::endl;
    for (int i = 0; i < videos.size(); i++) {
        std::cout << "  ";
        videos[i].printVideo();
        if (videos[i].isFlagged()) {
            std::cout << " - FLAGGED (reason: " << videos[i].getFlagReason() << ")";
        }
        if (i != videos.size() - 1) { std::cout << std::endl; }
    }
}

void VideoPlayer::playVideo(const std::string& videoId) {
    const Video* request = mVideoLibrary.getVideo(videoId);
    if (request == nullptr) {
        std::cout << "Cannot play video: Video does not exist" << std::endl;
    }
    else {
        if (request->isFlagged()) {
            std::cout << "Cannot play video: Video is currently flagged (reason: " << request->getFlagReason() << ")" << std::endl;
        }
        else if (isPlaying) {
            std::cout << "Stopping video: " << currentlyPlaying->getTitle() << std::endl <<
                "Playing video: " << request->getTitle() << std::endl;
        }
        else {
            std::cout << "Playing video: " << request->getTitle() << std::endl;
        }
        isPaused = false;
        isPlaying = true;
        currentlyPlaying = request;
    }
}

void VideoPlayer::stopVideo() {
    if (!isPlaying) {
        std::cout << "Cannot stop video: No video is currently playing" << std::endl;
    }
    else {
        std::cout << "Stopping video: " << currentlyPlaying->getTitle() << std::endl;
        isPlaying = false;
        isPaused = false;
    }
}

void VideoPlayer::playRandomVideo() {
    // Technically not random as the seed is the same everytime, but it works
    std::vector<Video> videos = mVideoLibrary.getVideos();
    double randNum = std::rand() % 10 / 10.0;
    float randIndex = randNum * (videos.size() - 1);
    while (videos.size() != 0) {
        if (videos[std::round(randIndex)].isFlagged()) {
            videos.erase(videos.begin() + std::round(randIndex));
            if (videos.size() == 0) { break; }
            double randNum = std::rand() % 10 / 10.0;
            float randIndex = randNum * (videos.size() - 1);
        }
        else { break; }
    }
    if (videos.size() == 0) {
        std::cout << "No videos available" << std::endl;
    }
    else {
        playVideo(videos[std::round(randIndex)].getVideoId());
    }
}

void VideoPlayer::pauseVideo() {
    if (!isPlaying) {
        std::cout << "Cannot pause video: No video is currently playing" << std::endl;
    }
    else {
        if (!isPaused) {
            std::cout << "Pausing video: " << currentlyPlaying->getTitle() << std::endl;
            isPaused = true;
        }
        else {
            std::cout << "Video already paused: " << currentlyPlaying->getTitle() << std::endl;
        }
    }
}

void VideoPlayer::continueVideo() {
    if (!isPlaying) {
        std::cout << "Cannot continue video: No video is currently playing" << std::endl;
    }
    else {
        if (isPaused) {
            std::cout << "Continuing video: " << currentlyPlaying->getTitle() << std::endl;
            isPaused = false;
        }
        else {
            std::cout << "Cannot continue video: Video is not paused" << std::endl;
        }
    }
}

void VideoPlayer::showPlaying() {
    if (isPlaying) {
        std::cout << "Currently playing: ";
        currentlyPlaying->printVideo();
        if (isPaused) {
            std::cout << " - PAUSED";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "No video is currently playing" << std::endl;
    }
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
    // This seems to be a clunky way of doing it, but it works
    bool playlistNameValid = true;
    // Check for whitespace - the commandparser already handles this, but it's there just incase
    for (char ch : playlistName) {
        if (ch == ' ') { playlistNameValid = false; }
    }

    // Check for if user inputted playlistName is already in use
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool nameValid = false;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    nameValid = true;
                    break;
                }
            }
            if (!nameValid) {
                playlistNameValid = false;
                break;
            }
        }
    }

    VideoPlaylist newPlaylist;
    if (playlistNameValid) {
        mVideoPlaylists.emplace(playlistName, std::move(newPlaylist));
        std::cout << "Successfully created new playlist: " << playlistName << std::endl;
    }
    else {
        // note that this will also be called if the playlist name includes whitespace
        std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
    }
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    // Using same code with slight alterations to test if the playlist exists
    bool playlistExists = false;
    std::string playlistNameCorrected;
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool isCurrentPlaylist = true;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    isCurrentPlaylist = false;
                    break;
                }
            }
            if (isCurrentPlaylist) {
                playlistNameCorrected = playlist.first;
                playlistExists = true;
                break;
            }
        }
    }

    if (!playlistExists) {
        std::cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else {
        auto search = mVideoPlaylists.find(playlistNameCorrected);
        Video* newVideo = mVideoLibrary.getVideo(videoId);
        if (newVideo == nullptr) {
            std::cout << "Cannot add video to " << playlistName << ": Video does not exist" << std::endl;
        }
        else if (newVideo->isFlagged()) {
            std::cout << "Cannot add video to " << playlistName << ": Video is currently flagged (reason: " << newVideo->getFlagReason() << ")" << std::endl;
        }
        else {
            if (!search->second.addVideo(videoId)) {
                std::cout << "Cannot add video to " << playlistName << ": Video already added" << std::endl;
            }
            else {
                std::cout << "Added video to " << playlistName << ": " << newVideo->getTitle() << std::endl;
            }
        }
    }
}

void VideoPlayer::showAllPlaylists() {
    if (mVideoPlaylists.size() == 0) {
        std::cout << "No playlists exist yet" << std::endl;
    }
    else {
        std::cout << "Showing all playlists:";
        for (const auto& playlist : mVideoPlaylists) {
            std::cout << std::endl << "  " << playlist.first;
        }
        std::cout << std::endl;
    }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
    // Using same code from addVideoToPlaylist
    bool playlistExists = false;
    std::string playlistNameCorrected;
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool isCurrentPlaylist = true;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    isCurrentPlaylist = false;
                    break;
                }
            }
            if (isCurrentPlaylist) {
                playlistNameCorrected = playlist.first;
                playlistExists = true;
                break;
            }
        }
    }

    if (!playlistExists) {
        std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else {
        auto search = mVideoPlaylists.find(playlistNameCorrected);
        std::cout << "Showing playlist: " << playlistName;
        std::vector<std::string> playlistVideos = search->second.getVideos();
        for (std::string videoId : playlistVideos) {
            Video* video = mVideoLibrary.getVideo(videoId);
            std::cout << std::endl << "  ";
            video->printVideo();
            if (video->isFlagged()) {
                std::cout << " - FLAGGED (reason: " << video->getFlagReason() << ")";
            }
        }
        if (playlistVideos.size() == 0) {
            std::cout << std::endl << "  " << "No videos here yet";
        }
        std::cout << std::endl;
    }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
    // Using same code from addVideoToPlaylist
    bool playlistExists = false;
    std::string playlistNameCorrected;
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool isCurrentPlaylist = true;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    isCurrentPlaylist = false;
                    break;
                }
            }
            if (isCurrentPlaylist) {
                playlistNameCorrected = playlist.first;
                playlistExists = true;
                break;
            }
        }
    }

    if (!playlistExists) {
        std::cout << "Cannot remove video from " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else if (mVideoLibrary.getVideo(videoId) == nullptr) {
        std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
    }
    else {
        auto search = mVideoPlaylists.find(playlistNameCorrected);
        if (search->second.removeVideo(videoId)) {
            std::cout << "Removed video from " << playlistName << ": " << mVideoLibrary.getVideo(videoId)->getTitle() << std::endl;
        }
        else {
            std::cout << "Cannot remove video from " << playlistName << ": Video is not in playlist" << std::endl;
        }
    }
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
    // Using same code from addVideoToPlaylist
    bool playlistExists = false;
    std::string playlistNameCorrected;
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool isCurrentPlaylist = true;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    isCurrentPlaylist = false;
                    break;
                }
            }
            if (isCurrentPlaylist) {
                playlistNameCorrected = playlist.first;
                playlistExists = true;
                break;
            }
        }
    }

    if (!playlistExists) {
        std::cout << "Cannot clear playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else {
        auto search = mVideoPlaylists.find(playlistNameCorrected);
        search->second.clearVideos();
        std::cout << "Successfully removed all videos from " << playlistName << std::endl;
    }
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
    // Using same code from addVideoToPlaylist
    bool playlistExists = false;
    std::string playlistNameCorrected;
    for (const auto& playlist : mVideoPlaylists) {
        std::string test = playlist.first;
        bool isCurrentPlaylist = true;
        if (test.size() == playlistName.size()) {
            for (int i = 0; i < test.size(); i++) {
                if (std::tolower(test[i]) != std::tolower(playlistName[i])) {
                    isCurrentPlaylist = false;
                    break;
                }
            }
            if (isCurrentPlaylist) {
                playlistNameCorrected = playlist.first;
                playlistExists = true;
                break;
            }
        }
    }

    if (!playlistExists) {
        std::cout << "Cannot delete playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
    else {
        mVideoPlaylists.erase(playlistNameCorrected);
        std::cout << "Deleted playlist: " << playlistName << std::endl;
    }
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
    // Converting search term to lowercase to avoid being case sensitive
    std::string testSearchTerm = searchTerm;
    std::for_each(testSearchTerm.begin(), testSearchTerm.end(), [](char& c) { c = ::tolower(c); });
    int result = 0;
    std::vector<int> videoIndex;
    int indexTrack = 0;
    const std::vector<Video> videos = mVideoLibrary.getVideos();
    for (Video video : videos) {
        if (!video.isFlagged()) {
            std::string title = video.getTitle();
            std::for_each(title.begin(), title.end(), [](char& c) { c = ::tolower(c); });

            std::size_t found = title.find(testSearchTerm);

            if (found != std::string::npos) { // if search term found
                if (!result) {
                    std::cout << "Here are the results for " << searchTerm << ":";
                }
                result++;
                videoIndex.emplace_back(indexTrack);
                std::cout << std::endl << "  " << result << ") ";
                video.printVideo();
            }
        }
        indexTrack++;
    }
    if (result == 0) {
        std::cout << "No search results for " << searchTerm << std::endl;
    }
    else {
        std::cout << std::endl << "Would you like to play any of the above? If yes, specify the number of the video." <<
            "\nIf your answer is not a valid number, we will assume it's a no." << std::endl;
        std::string input;
        std::getline(std::cin, input);
        bool isNumber = true;
        for (char ch : input) {
            if (!isdigit(ch)) {
                isNumber = false;
                break;
            }
        }
        if (isNumber) {
            int videoNumber = stoi(input);
            if (videoNumber > 0 && videoNumber < videoIndex.size() + 1) {
                videoNumber -= 1;
                playVideo(videos[videoIndex[videoNumber]].getVideoId());
            }
        }
    }
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
    // Remove case sensitivity
    std::string testVideoTag = videoTag;
    std::for_each(testVideoTag.begin(), testVideoTag.end(), [](char& c) { c = ::tolower(c); });

    // Tracking results
    std::vector<Video> videos = mVideoLibrary.getVideos();
    int results = 0;
    std::vector<int> resultIndex;
    int currentIndex = 0;

    for (Video video : videos) {
        if (!video.isFlagged()) {
            for (std::string tag : video.getTags()) {
                if (tag == testVideoTag) {
                    resultIndex.emplace_back(currentIndex);
                    results++;
                    break;
                }
            }
        }
        currentIndex++;
    }

    // Displaying results
    if (results == 0) {
        std::cout << "No search results for " << videoTag << std::endl;
    }
    else {
        std::cout << "Here are the results for " << videoTag << ":";
        for (int i = 0; i < resultIndex.size(); i++) {
            std::cout << std::endl << (i + 1) << ") ";
            videos[resultIndex[i]].printVideo();
        }
        std::cout << std::endl << "Would you like to play any of the above? If yes, specify the number of the video." <<
            "\nIf your answer is not a valid number, we will assume it's a no." << std::endl;
        std::string input;
        std::getline(std::cin, input);
        bool isNumber = true;
        for (char ch : input) {
            if (!isdigit(ch)) {
                isNumber = false;
                break;
            }
        }
        if (isNumber) {
            int videoNumber = stoi(input);
            if (videoNumber > 0 && videoNumber < resultIndex.size() + 1) {
                videoNumber -= 1;
                playVideo(videos[resultIndex[videoNumber]].getVideoId());
            }
        }
    }
}

void VideoPlayer::flagVideo(const std::string& videoId) {
    Video* video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
        std::cout << "Cannot flag video: Video does not exist" << std::endl;
    }
    else {
        if (!video->isFlagged()) {
            video->flag();
            if (currentlyPlaying == video) {
                stopVideo();
                currentlyPlaying = nullptr;
            }
            std::cout << "Successfully flagged video: " << video->getTitle() << " (reason: " << video->getFlagReason() << ")" << std::endl;
        }
        else {
            std::cout << "Cannot flag video: Video is already flagged" << std::endl;
        }
    }
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
    Video* video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
        std::cout << "Cannot flag video: Video does not exist" << std::endl;
    }
    else {
        if (!video->isFlagged()) {
            video->flag(reason);
            if (currentlyPlaying == video) {
                stopVideo();
                currentlyPlaying = nullptr;
            }
            std::cout << "Successfully flagged video: " << video->getTitle() << " (reason: " << video->getFlagReason() << ")" << std::endl;
        }
        else {
            std::cout << "Cannot flag video: Video is already flagged" << std::endl;
        }
    }
}

void VideoPlayer::allowVideo(const std::string& videoId) {
    Video* video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
        std::cout << "Cannot remove flag from video: Video does not exist" << std::endl;
    }
    else {
        if (!video->isFlagged()) {
            std::cout << "Cannot remove flag from video: Video is not flagged" << std::endl;
        }
        else {
            video->removeFlag();
            std::cout << "Successfully removed flag from video: " << video->getTitle() << std::endl;
        }
    }
}
