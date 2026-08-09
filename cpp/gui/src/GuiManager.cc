#include "GuiManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace LahmaPlayer::Gui
{
    GuiManager::GuiManager() : m_running(false), m_isPlaying(false), m_progress(0.0), m_selectedFileIndex(0)
    {
        m_screen = std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
        m_logFile.open("gui_manager.log", std::ofstream::out | std::ofstream::app);
        logToFile("GuiManager initialized");
        updateAudioFileList();
    }

    GuiManager::~GuiManager()
    {
        stopLoop();
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    void GuiManager::startLoop()
    {
        m_running = true;
        
        auto file_picker = createFilePickerComponent();
        
        // Create a lambda to handle play/pause toggle
        auto play_button = ftxui::Button("Play", [this] { 
            logToFile("Play/Pause button clicked");
            // Make sure we have a valid file selected before calling loadAudioFile
            if (m_audioFiles.empty()) {
                logToFile("No audio files found to play");
                return;
            }
            // If the selected index is out of bounds, reset to first file
            if (m_selectedFileIndex < 0 || m_selectedFileIndex >= static_cast<int>(m_audioFiles.size())) {
                m_selectedFileIndex = 0;
            }
            
            // Toggle play/pause
            if (m_isPlaying) {
                stopPlayback();
            } else {
                // If we don't have an audio source loaded, load it first
                if (!m_audioSource) {
                    loadAudioFile();
                }
                startPlayback();
            }
        });
        
        auto stop_button = ftxui::Button("Stop", [this] { 
            stopPlayback();
            m_audioSource.reset();
        });
        auto refresh_button = ftxui::Button("Refresh Files", [this] { 
            updateAudioFileList(); 
        });
        auto exit_button = ftxui::Button("Exit", [this] {
            stopLoop();
            // Force exit by creating an exit event
            m_screen->Exit();
        });
        
        // Create separate sections for file picker and controls
        auto file_picker_section = ftxui::Container::Vertical({
            file_picker
        });
        
        auto controls_section = ftxui::Container::Vertical({
            play_button,
            stop_button,
            refresh_button,
            exit_button
        });
        
        // Create a horizontal layout with file picker on the left and controls on the right
        auto main_container = ftxui::Container::Horizontal({
            file_picker_section | ftxui::flex_grow,
            controls_section | ftxui::flex_grow
        });
        
        // Set the root component
        m_screen->Loop(main_container);
    }

    void GuiManager::stopLoop()
    {
        m_running = false;
    }

    void GuiManager::loadAudioFile()
    {
        logToFile("loadAudioFile called");
        logToFile("Selected file index: " + std::to_string(m_selectedFileIndex));
        logToFile("Audio files count: " + std::to_string(m_audioFiles.size()));
        
        // Stop any current playback
        stopPlayback();
        
        // Destroy the current audio source
        m_audioSource.reset();
        
        // Check if a file is selected
        if (m_selectedFileIndex >= 0 && m_selectedFileIndex < static_cast<int>(m_audioFiles.size()))
        {
            m_fileName = m_audioFiles[m_selectedFileIndex];
            logToFile("Selected file: " + m_fileName);
        }
        else
        {
            logToFile("No valid file selected");
            return;
        }
        
        // Create new audio source
        m_audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(m_fileName);
        if (!m_audioSource)
        {
            std::cout << "Failed to create audio source for file: " << m_fileName << std::endl;
            logToFile("Failed to create audio source for file: " + m_fileName);
            return;
        }
        
        // Create DSP engine and audio stream
        m_dspEngine = std::make_shared<LahmaPlayer::DspEngine::DspEngine>(m_audioSource);
        m_audioStream = std::make_shared<LahmaPlayer::AudioStream::AudioStream>();
        logToFile("Audio file loaded successfully: " + m_fileName);
    }

    void GuiManager::startPlayback()
    {
        logToFile("startPlayback called");
        if (!m_audioStream)
        {
            std::cout << "No audio stream available" << std::endl;
            logToFile("No audio stream available");
            return;
        }
        
        // Stop any currently playing audio
        stopPlayback();
        
        m_isPlaying = true;
        m_progress = 0.0;
        
        // Start the audio stream
        m_audioStream->start(m_audioSource);
        logToFile("Playback started for file: " + m_fileName);
    }

    void GuiManager::stopPlayback()
    {
        m_isPlaying = false;
        m_progress = 0.0;
        
        // Stop the audio stream if it's active
        if (m_audioStream)
        {
            m_audioStream->stop();
            m_audioStream->waitUntilFinished();
        }
    }

    void GuiManager::updateAudioFileList()
    {
        m_audioFiles.clear();
        
        try {
            // Get current directory
            auto current_path = std::filesystem::current_path();
            
            // Iterate through directory entries
            for (const auto& entry : std::filesystem::directory_iterator(current_path))
            {
                if (entry.is_regular_file() && isAudioFile(entry.path().filename().string()))
                {
                    m_audioFiles.push_back(entry.path().filename().string());
                }
            }
            
            // Sort audio files alphabetically
            std::sort(m_audioFiles.begin(), m_audioFiles.end());
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cout << "Error reading directory: " << ex.what() << std::endl;
        }
        
        m_selectedFileIndex = 0;
    }

    void GuiManager::logToFile(const std::string& message)
    {
        if (m_logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            m_logFile << "[" << std::ctime(&time_t) << "] " << message << std::endl;
            m_logFile.flush(); // Ensure immediate writing to file
        }
    }

    bool GuiManager::isAudioFile(const std::string& filename)
    {
        // Use AudioSourceFactory to determine if the file is a supported audio file
        auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(filename);
        
        // Return true if we successfully created an audio source (not nullptr)
        return audioSource != nullptr;
    }

    ftxui::Component GuiManager::createFilePickerComponent()
    {
        // Create a menu for audio files
        auto menu = ftxui::Menu(&m_audioFiles, &m_selectedFileIndex);
        
        // Create a renderer for the menu that shows selected file
        auto renderer = ftxui::Renderer(menu, [this, menu] {
            return ftxui::vbox({
                ftxui::text("Select audio file:") | ftxui::bold,
                ftxui::separator(),
                menu->Render() | ftxui::frame
            });
        });
        
        return renderer;
    }
}