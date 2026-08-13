#include "AudioManager.h"
#include <iostream>
#include <filesystem>

namespace LahmaPlayer::Gui
{
    AudioManager::AudioManager()
    {
        LahmaPlayer::Logger::getInstance().info("AudioManager initialized");
    }

    AudioManager::~AudioManager()
    {
    }

    bool AudioManager::loadAudioFile(const std::string& fileName)
    {
        LahmaPlayer::Logger::getInstance().info("loadAudioFile called for: " + fileName);
        
        // Stop any current playback
        stopPlayback();
        
        // Create audio source
        m_audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(fileName);
        if (!m_audioSource)
        {
            LahmaPlayer::Logger::getInstance().warning("Failed to create audio source for file: " + fileName);
            return false;
        }
        
        // Create DSP engine with audio source
        m_dspEngine = std::make_shared<LahmaPlayer::DspEngine::DspEngine>(m_audioSource);
        
        // Create audio stream
        m_audioStream = std::make_shared<LahmaPlayer::AudioStream::AudioStream>();
        
        LahmaPlayer::Logger::getInstance().info("Audio file loaded successfully: " + fileName);
        return true;
    }

    void AudioManager::startPlayback()
    {
        LahmaPlayer::Logger::getInstance().info("startPlayback called");
        if (!m_audioStream)
        {
            std::cout << "No audio stream available" << std::endl;
            LahmaPlayer::Logger::getInstance().warning("No audio stream available");
            return;
        }
        
        // Start the audio stream
        m_audioStream->start(m_audioSource);
        LahmaPlayer::Logger::getInstance().info("Playback started");
    }

    void AudioManager::stopPlayback()
    {
        LahmaPlayer::Logger::getInstance().info("stopPlayback called");
        
        // Stop the audio stream if it's active
        if (m_audioStream)
        {
            m_audioStream->stop();
            m_audioStream->waitUntilFinished();
        }
    }

}