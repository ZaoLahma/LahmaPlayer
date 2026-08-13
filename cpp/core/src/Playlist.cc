#include "Playlist.h"
#include "AudioSourceFactory.h"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <fstream>

namespace LahmaPlayer {

Playlist::Playlist()
    : m_currentIndex(std::size_t(-1))
{
    m_logFile.open("playlist.log", std::ofstream::out | std::ofstream::app);
    logToFile("Playlist initialized");
}

Playlist::~Playlist()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Playlist::logToFile(const std::string& message)
{
    if (m_logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        m_logFile << "[" << std::ctime(&time_t) << "] Playlist: " << message << std::endl;
        m_logFile.flush();
    }
}

bool Playlist::loadFromDirectory(const std::string& dirPath)
{
    logToFile("loadFromDirectory called for: " + dirPath);
    
    // Check if path exists and is a directory
    if (!std::filesystem::is_directory(dirPath)) {
        logToFile("ERROR: Path is not a directory");
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
        return false;
    }

    // Sort files alphabetically for consistent ordering
    std::sort(playableFiles.begin(), playableFiles.end());

    // Load each playable file
    m_tracks.clear();
    for (const auto& fileName : playableFiles) {
        auto source = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(fileName);
        if (source) {
            m_tracks.push_back({fileName, source});
        }
    }

    // Reset index to 0 after loading
    m_currentIndex = 0;

    return !m_tracks.empty();
}


bool Playlist::advanceToNext()
{
    logToFile("advanceToNext called, current: " + std::to_string(m_currentIndex));
    
    if (m_currentIndex < m_tracks.size()) {
        m_currentIndex++;
        logToFile("advanceToNext: moved to index " + std::to_string(m_currentIndex));
    } else {
        logToFile("advanceToNext: already at end");
    }
    
    return m_currentIndex < m_tracks.size();
}

void Playlist::reset()
{
    logToFile("reset called");
    m_currentIndex = 0;
}

} // namespace LahmaPlayer
