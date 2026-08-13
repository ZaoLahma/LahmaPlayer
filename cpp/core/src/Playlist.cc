#include "Playlist.h"
#include "AudioSourceFactory.h"
#include "Logger.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace LahmaPlayer {

Playlist::Playlist()
    : m_currentIndex(std::size_t(-1))
{
    LahmaPlayer::Logger::getInstance().info("Playlist initialized");
}

Playlist::~Playlist()
{
}

bool Playlist::loadFromDirectory(const std::string& dirPath)
{
    LahmaPlayer::Logger::getInstance().info("loadFromDirectory called for: " + dirPath);
    
    // Check if path exists and is a directory
    if (!std::filesystem::is_directory(dirPath)) {
        LahmaPlayer::Logger::getInstance().warning("ERROR: Path is not a directory");
        return false;
    }

    // Scan directory for playable files
    std::vector<std::string> playableFiles;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            std::string path = entry.path().string();
            
            // Try to create audio source - factory will return nullptr for unsupported files
            auto source = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(path);
            if (source) {
                playableFiles.push_back(path);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
        LahmaPlayer::Logger::getInstance().error("Error scanning directory: " + std::string(e.what()));
        return false;
    }

    // Sort files alphabetically for consistent ordering
    std::sort(playableFiles.begin(), playableFiles.end());

    // Load each playable file
    m_tracks.clear();
    for (const auto& fileName : playableFiles) {
        m_tracks.push_back({fileName});
    }

    // Reset index to 0 after loading
    m_currentIndex = 0;

    return !m_tracks.empty();
}


bool Playlist::advanceToNext()
{
    LahmaPlayer::Logger::getInstance().info("advanceToNext called, current: " + std::to_string(getCurrentIndex()));
    
    if (m_currentIndex < m_tracks.size()) {
        m_currentIndex++;
        LahmaPlayer::Logger::getInstance().info("advanceToNext: moved to index " + std::to_string(m_currentIndex));
    } else {
        LahmaPlayer::Logger::getInstance().info("advanceToNext: already at end");
    }
    
    return m_currentIndex < m_tracks.size();
}

bool Playlist::advanceToPrevious()
{
    LahmaPlayer::Logger::getInstance().info("advanceToPrevious called, current: " + std::to_string(getCurrentIndex()));
    
    if (m_currentIndex > 0) {
        m_currentIndex--;
        LahmaPlayer::Logger::getInstance().info("advanceToPrevious: moved to index " + std::to_string(m_currentIndex));
    } else {
        LahmaPlayer::Logger::getInstance().info("advanceToPrevious: already at start");
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

} // namespace LahmaPlayer
