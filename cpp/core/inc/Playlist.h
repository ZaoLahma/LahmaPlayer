#pragma once

#include "AudioSource.h"
#include "Logger.h"
#include <cstdint>
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

    /**
     * @struct TrackPosition
     * @brief Represents position within a track
     */
    struct TrackPosition
    {
        size_t trackIndex = std::size_t(-1);
        uint32_t samplePosition = 0;
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

    /**
     * @brief Seek within current track
     * @param numSamples Number of samples to seek
     * @param direction Seek direction
     * @return true if seek was successful
     */
    bool seekTrack(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction);

    /**
     * @brief Get current sample position in track
     * @return Current sample position, or 0 if no track loaded
     */
    uint32_t getCurrentTrackPosition() const;

  private:
    std::vector<Track> m_tracks;
    size_t m_currentIndex = std::size_t(-1);
    uint32_t m_currentPosition = 0;
};

} // namespace LahmaPlayer
