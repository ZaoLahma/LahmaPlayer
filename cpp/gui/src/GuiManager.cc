#include "GuiManager.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace LahmaPlayer::Gui
{
    GuiManager::GuiManager() : m_running(false), m_isPlaying(false), m_progress(0.0)
    {
        m_screen = std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
        m_component = createMainComponent();
    }

    GuiManager::~GuiManager()
    {
        stopLoop();
    }

    void GuiManager::startLoop()
    {
        m_running = true;
        
        // Start the FTXUI screen with the main component
        m_screen->Loop(m_component);
    }

    void GuiManager::stopLoop()
    {
        m_running = false;
    }

    void GuiManager::loadAudioFile()
    {
        // Stop any current playback
        stopPlayback();
        
        // Destroy the current audio source
        m_audioSource.reset();
        
        // Create new audio source
        m_audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(m_fileName);
        if (!m_audioSource)
        {
            std::cout << "Failed to create audio source for file: " << m_fileName << std::endl;
            return;
        }
        
        // Create DSP engine and audio stream
        m_dspEngine = std::make_shared<LahmaPlayer::DspEngine::DspEngine>(m_audioSource);
        m_audioStream = std::make_shared<LahmaPlayer::AudioStream::AudioStream>();
    }

    void GuiManager::startPlayback()
    {
        if (!m_audioStream)
        {
            std::cout << "No audio stream available" << std::endl;
            return;
        }
        
        m_isPlaying = true;
        m_progress = 0.0;
        
        // Start the audio stream
        m_audioStream->start(m_audioSource);
    }

    void GuiManager::stopPlayback()
    {
        m_isPlaying = false;
        m_progress = 0.0;
    }

    ftxui::Component GuiManager::createMainComponent()
    {
        auto input = ftxui::Input(&m_fileName, "Enter audio file path");
        auto load_button = ftxui::Button("Load", [this] { loadAudioFile(); });
        auto play_button = ftxui::Button("Play", [this] { startPlayback(); });
        auto stop_button = ftxui::Button("Stop", [this] { stopPlayback(); });
        
        return ftxui::Container::Vertical({
            input,
            load_button,
            play_button,
            stop_button
        });
    }
}