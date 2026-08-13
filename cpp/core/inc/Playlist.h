#pragma once

#include "AudioSource.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>

namespace LahmaPlayer {

/**
 * @class Playlist
 * @brief Manages a queue of audio tracks
 */
class Playlist
{
public:
    /**
     * @struct Track
     * @brief Represents an audio track
     */
    struct Track
    {
        std::string fileName;
        std::shared_ptr<AudioSource::AudioSource> source;
        size_t currentIndex = 0;  // For seeking within track
    };

    Playlist();
    ~Playlist();

    /**
     * @brief Load all playable files from a directory
     * @param dirPath Path to directory containing audio files
     * @return true if directory contains playable files
     */
    bool loadFromDirectory(const std::string& dirPath);

    /**
     * @brief Check if there are more tracks available
     * @return true if there are more tracks
     */
    bool hasMore() const { return m_currentIndex < m_tracks.size(); }

    /**
     * @brief Get current track (mutable)
     * @return Reference to current track
     */
    Track& currentTrack() { return m_tracks[m_currentIndex]; }

    /**
     * @brief Get current track (const)
     * @return Const reference to current track
     */
    const Track& currentTrack() const { return m_tracks[m_currentIndex]; }

    /**
     * @brief Advance to next track
     * @return true if advancement was successful, false if at end
     */
    bool advanceToNext();

    /**
     * @brief Reset playlist to first track
     */
    void reset();

    /**
     * @brief Get track count
     * @return Number of tracks in playlist
     */
    size_t size() const { return m_tracks.size(); }

    /**
     * @brief Check if at end of playlist
     * @return true if at end
     */
    bool isAtEnd() const { return !hasMore(); }

    /**
     * @brief Get current track index
     * @return Current track index, or -1 if no track
     */
    ssize_t getCurrentTrackIndex() const { return static_cast<ssize_t>(m_currentIndex); }

    /**
     * @brief Get track count
     * @return Number of tracks in playlist
     */
    size_t getTrackCount() const { return m_tracks.size(); }

    /**
     * @brief Log a message to file
     */
    void logToFile(const std::string& message);

    private:
        std::vector<Track> m_tracks;
        size_t m_currentIndex = std::size_t(-1);
        std::ofstream m_logFile;
};

} // namespace LahmaPlayer
