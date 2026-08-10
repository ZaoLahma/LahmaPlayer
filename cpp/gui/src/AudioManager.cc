#include "AudioManager.h"
#include <iostream>
#include <filesystem>
#include <chrono>

namespace LahmaPlayer::Gui
{
    AudioManager::AudioManager()
    {
        m_logFile.open("audio_manager.log", std::ofstream::out | std::ofstream::app);
        logToFile("AudioManager initialized");
    }

    AudioManager::~AudioManager()
    {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    bool AudioManager::loadAudioFile(const std::string& fileName)
    {
        logToFile("loadAudioFile called for: " + fileName);
        
        // Stop any current playback
        stopPlayback();
        
        // Create audio source
        m_audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(fileName);
        if (!m_audioSource)
        {
            logToFile("Failed to create audio source for file: " + fileName);
            return false;
        }
        
        // Create DSP engine with audio source
        m_dspEngine = std::make_shared<LahmaPlayer::DspEngine::DspEngine>(m_audioSource);
        
        // Create audio stream
        m_audioStream = std::make_shared<LahmaPlayer::AudioStream::AudioStream>();
        
        logToFile("Audio file loaded successfully: " + fileName);
        return true;
    }

    void AudioManager::startPlayback()
    {
        logToFile("startPlayback called");
        if (!m_audioStream)
        {
            std::cout << "No audio stream available" << std::endl;
            logToFile("No audio stream available");
            return;
        }
        
        // Start the audio stream
        m_audioStream->start(m_audioSource);
        logToFile("Playback started");
    }

    void AudioManager::stopPlayback()
    {
        logToFile("stopPlayback called");
        
        // Stop the audio stream if it's active
        if (m_audioStream)
        {
            m_audioStream->stop();
            m_audioStream->waitUntilFinished();
        }
    }

    void AudioManager::logToFile(const std::string& message)
    {
        if (m_logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            m_logFile << "[" << std::ctime(&time_t) << "] " << message << std::endl;
            m_logFile.flush(); // Ensure immediate writing to file
        }
    }
}