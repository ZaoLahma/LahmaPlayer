#include "ControlsComponent.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

namespace LahmaPlayer::Gui
{
    ControlsComponent::ControlsComponent() : m_isPlaying(false), m_progress(0.0)
    {
        m_logFile.open("controls_component.log", std::ofstream::out | std::ofstream::app);
        logToFile("ControlsComponent initialized");
    }

    ControlsComponent::~ControlsComponent()
    {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    ftxui::Component ControlsComponent::createComponent()
    {
        // Create buttons for controls
        auto play_button = ftxui::Button("Play", [this] { 
            logToFile("Play/Pause button clicked");

            // Toggle play/pause using callback
            if (m_playPauseCallback) {
                m_playPauseCallback();
            }
        });
        
        auto stop_button = ftxui::Button("Stop", [this] { 
            logToFile("Stop button clicked");
            // Stop using callback
            if (m_stopCallback) {
                m_stopCallback();
            }
            // Clear the audio source reference
            m_audioSource.reset();
        });
        
        auto refresh_button = ftxui::Button("Refresh Files", [this] { 
            // This will be handled by the main GuiManager
        });
        
        auto exit_button = ftxui::Button("Exit", [this] {
            logToFile("Exit button clicked");
            // Stop playback and exit using callback
            if (m_stopCallback) {
                m_stopCallback();
            }
            // Call exit callback if set
            if (m_exitCallback) {
                m_exitCallback();
            }
        });
        
        // Create the controls component
        auto controls = ftxui::Container::Vertical({
            play_button,
            stop_button,
            refresh_button,
            exit_button
        });
        
        return ftxui::Renderer(controls, [controls] {
            return ftxui::vbox({
                ftxui::text("Controls:") | ftxui::bold,
                ftxui::separator(),
                controls->Render() | ftxui::frame
            });
        });
    }

    void ControlsComponent::setAudioSource(std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> audioSource)
    {
        m_audioSource = audioSource;
    }

    void ControlsComponent::setAudioStream(std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> audioStream)
    {
        m_audioStream = audioStream;
    }

    void ControlsComponent::setFileName(const std::string& fileName)
    {
        m_fileName = fileName;
    }

    void ControlsComponent::setIsPlaying(bool isPlaying)
    {
        m_isPlaying = isPlaying;
    }

    void ControlsComponent::loadAudioFile(const std::string& fileName)
    {
        logToFile("loadAudioFile called for file: " + fileName);
        m_fileName = fileName;
        // Note: The actual audio loading is now handled by AudioManager
        // This method just updates the file name in the controls
    }

    void ControlsComponent::logToFile(const std::string& message)
    {
        if (m_logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            m_logFile << "[" << std::ctime(&time_t) << "] " << message << std::endl;
            m_logFile.flush(); // Ensure immediate writing to file
        }
    }
}