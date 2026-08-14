#include "Playlist.h"
#include "AudioSourceFactory.h"
#include "Logger.h"
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace LahmaPlayer
{

Playlist::Playlist() : m_currentIndex(std::size_t(-1))
{
    LahmaPlayer::Logger::getInstance().info("Playlist initialized");
}

Playlist::~Playlist() {}

bool Playlist::loadFromDirectory(const std::string &dirPath)
{
    LahmaPlayer::Logger::getInstance().info("loadFromDirectory called for: " + dirPath);

    // Check if path exists and is a directory
    if (!std::filesystem::is_directory(dirPath))
    {
        LahmaPlayer::Logger::getInstance().warning("ERROR: Path is not a directory");
        return false;
    }

    // Scan directory for playable files
    std::vector<std::string> playableFiles;

    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(dirPath))
        {
            std::string path = entry.path().string();

            // Try to create audio source - factory will return nullptr for unsupported files
            auto source = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(path);
            if (source)
            {
                playableFiles.push_back(path);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
        LahmaPlayer::Logger::getInstance().error("Error scanning directory: " + std::string(e.what()));
        return false;
    }

    // Sort files alphabetically for consistent ordering
    std::sort(playableFiles.begin(), playableFiles.end());

    // Load each playable file
    m_tracks.clear();
    for (const auto &fileName : playableFiles)
    {
        m_tracks.push_back({fileName});
    }

    // Reset index to 0 after loading
    m_currentIndex = 0;

    return !m_tracks.empty();
}

bool Playlist::advanceToNext()
{
    LahmaPlayer::Logger::getInstance().info("advanceToNext called, current: " + std::to_string(getCurrentIndex()));

    if (m_currentIndex < m_tracks.size())
    {
        m_currentIndex++;
        LahmaPlayer::Logger::getInstance().info("advanceToNext: moved to index " + std::to_string(getCurrentIndex()));

        // Circular wrapping: if at end, wrap to first
        if (m_currentIndex >= static_cast<int>(m_tracks.size()))
        {
            m_currentIndex = 0;
            LahmaPlayer::Logger::getInstance().info("advanceToNext: wrapped to first track (index 0)");
        }
    }

    return m_currentIndex < static_cast<int>(m_tracks.size());
}

bool Playlist::advanceToPrevious()
{
    LahmaPlayer::Logger::getInstance().info("advanceToPrevious called, current: " + std::to_string(getCurrentIndex()));

    if (m_currentIndex > 0)
    {
        m_currentIndex--;
        LahmaPlayer::Logger::getInstance().info("advanceToPrevious: moved to index " +
                                                std::to_string(getCurrentIndex()));
    }
    else
    {
        LahmaPlayer::Logger::getInstance().info("advanceToPrevious: already at start");

        // Circular wrapping: if at start, wrap to last
        if (!m_tracks.empty())
        {
            m_currentIndex = static_cast<int>(m_tracks.size()) - 1;
            LahmaPlayer::Logger::getInstance().info("advanceToPrevious: wrapped to last track (index " +
                                                    std::to_string(m_currentIndex) + ")");
        }
    }

    return m_currentIndex >= 0;
}

void Playlist::reset()
{
    LahmaPlayer::Logger::getInstance().info("reset called");
    m_currentIndex = 0;
}

bool Playlist::isAtStart() const
{
    return m_currentIndex == 0;
}

bool Playlist::seekTrack(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction)
{
    if (m_currentIndex == std::size_t(-1) || m_currentIndex >= m_tracks.size())
    {
        LahmaPlayer::Logger::getInstance().warning("seekTrack: No track loaded");
        return false;
    }

    LahmaPlayer::Logger::getInstance().info("seekTrack called, current position: " + std::to_string(m_currentPosition) +
                                            ", seeking: " + std::to_string(numSamples) + " samples " +
                                            (direction == LahmaPlayer::AudioSource::AudioSource::SeekDirection::Forward ? "forward" : "backward"));

    // Note: For playlist functionality, the actual seek is delegated to the audio source
    // when it's loaded through AudioManager. This method is a placeholder for future
    // implementation where we might want to seek within the current track without reloading.
    // For now, we just update the position tracking.
    
    // TODO: Implement proper seek by keeping audio source reference in Track struct
    return true;
}

uint32_t Playlist::getCurrentTrackPosition() const
{
    if (m_currentIndex == std::size_t(-1) || m_currentIndex >= m_tracks.size())
    {
        return 0;
    }

    return m_currentPosition;
}

} // namespace LahmaPlayer
