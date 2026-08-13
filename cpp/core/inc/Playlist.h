#pragma once

#include "AudioSource.h"
#include "Logger.h"
#include <memory>
#include <string>
#include <vector>

namespace LahmaPlayer
{

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
        size_t currentIndex = 0; // For seeking within track
    };

    Playlist();
    ~Playlist();

    /**
     * @brief Load all playable files from a directory
     * @param dirPath Path to directory containing audio files
     * @return true if directory contains playable files
     */
    bool loadFromDirectory(const std::string &dirPath);

    /**
     * @brief Check if there are more tracks available
     * @return true if there are more tracks
     */
    bool hasMore() const
    {
        return m_currentIndex < m_tracks.size();
    }

    /**
     * @brief Get current track filename (mutable)
     * @return Reference to current track's filename
     */
    const std::string &currentTrackFileName()
    {
        return m_tracks[m_currentIndex].fileName;
    }

    /**
     * @brief Get current track filename (const)
     * @return Const reference to current track's filename
     */
    const std::string &currentTrackFileName() const
    {
        return m_tracks[m_currentIndex].fileName;
    }

    /**
     * @brief Advance to next track
     * @return true if advancement was successful, false if at end
     */
    bool advanceToNext();

    /**
     * @brief Advance to previous track
     * @return true if advancement was successful, false if at start
     */
    bool advanceToPrevious();

    /**
     * @brief Reset playlist to first track
     */
    void reset();

    /**
     * @brief Check if at start of playlist
     * @return true if at first track
     */
    bool isAtStart() const;

    /**
     * @brief Get current track index
     * @return Current track index, or -1 if no track
     */
    ssize_t getCurrentIndex() const
    {
        return static_cast<ssize_t>(m_currentIndex);
    }

  private:
    std::vector<Track> m_tracks;
    size_t m_currentIndex = std::size_t(-1);
};

} // namespace LahmaPlayer
