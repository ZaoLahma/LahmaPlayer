#include "GuiManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace LahmaPlayer::Gui
{
    GuiManager::GuiManager() : m_isPlaying(false), m_hasAudioFileLoaded(false), m_progress(0.0)
    {
        m_screen = std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
        m_logFile.open("gui_manager.log", std::ofstream::out | std::ofstream::app);
        logToFile("GuiManager initialized");
        
        // Initialize audio manager
        m_audioManager = std::make_unique<AudioManager>();
        
        // Initialize components
        m_filePicker = std::make_unique<FilePickerComponent>();
        m_controls = std::make_unique<ControlsComponent>();
        
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
        // Set up callbacks for controls component
        m_controls->setPlayPauseCallback([this]() {
            if (m_isPlaying) {
                stopPlayback();
            } else {
                if (!m_hasAudioFileLoaded)
                {
                    loadAudioFile();
                }
                startPlayback();
            }
        });
        
        m_controls->setStopCallback([this]() {
            stopPlayback();
            m_hasAudioFileLoaded = false;
        });
        
        m_controls->setExitCallback([this]() {
            stopLoop();
        });
        
        // Set up callbacks for controls component (for file selection)
        m_controls->setFileSelectedCallback([this](const std::string& fileName) {
            logToFile("File selected in callback: " + fileName);
            m_audioManager->loadAudioFile(fileName);
        });
        
        m_filePicker->setRefreshCallback([this]() {
            updateAudioFileList();
        });
        
        m_filePicker->setExitCallback([this]() {
            stopLoop();
        });
        
        // Get the components
        auto file_picker = m_filePicker->createComponent();
        auto controls = m_controls->createComponent();
        
        // Create separate sections for file picker and controls
        auto file_picker_section = ftxui::Container::Vertical({
            file_picker
        });
        
        auto controls_section = ftxui::Container::Vertical({
            controls
        });
        
        // Create a horizontal layout with file picker on the left and controls on the right
        auto main_container = ftxui::Container::Horizontal({
            file_picker_section | ftxui::flex_grow,
            controls_section | ftxui::flex_grow
        });
        
        // Set the main component for the screen
        m_component = main_container;
        m_screen->Loop(m_component);
    }

    void GuiManager::stopLoop()
    {
        m_screen->Exit();
    }

    void GuiManager::loadAudioFile()
    {
        logToFile("loadAudioFile called");
        logToFile("Selected file index: " + std::to_string(m_filePicker->getSelectedFileIndex()));
        logToFile("Audio files count: " + std::to_string(m_filePicker->getAudioFiles().size()));
        
        // Stop any current playback
        stopPlayback();
        
        // Check if a file is selected
        int selected_index = m_filePicker->getSelectedFileIndex();
        if (selected_index >= 0 && selected_index < static_cast<int>(m_filePicker->getAudioFiles().size()))
        {
            std::string fileName = m_filePicker->getAudioFiles()[selected_index];
            logToFile("Selected file: " + fileName);
            
            // Load audio file through audio manager
            bool success = m_audioManager->loadAudioFile(fileName);
            
            if (!success)
            {
                logToFile("Failed to load audio file: " + fileName);
                return;
            }
            
            // Update controls with new audio source
            m_controls->setAudioSource(m_audioManager->getAudioSource());
            m_controls->setAudioStream(m_audioManager->getAudioStream());
            m_controls->setFileName(fileName);
            m_hasAudioFileLoaded = true;
            
            logToFile("Audio file loaded successfully: " + fileName);
        }
        else
        {
            logToFile("No valid file selected");
        }
    }

    void GuiManager::startPlayback()
    {
        logToFile("startPlayback called");
        if (!m_audioManager->getAudioStream())
        {
            std::cout << "No audio stream available" << std::endl;
            logToFile("No audio stream available");
            return;
        }
        
        // Stop any currently playing audio
        stopPlayback();
        
        m_isPlaying = true;
        m_progress = 0.0;
        
        // Update controls state
        m_controls->setIsPlaying(true);
        
        // Start the audio stream
        m_audioManager->startPlayback();
        logToFile("Playback started");
    }

    void GuiManager::stopPlayback()
    {
        m_isPlaying = false;
        m_progress = 0.0;
        
        // Update controls state
        m_controls->setIsPlaying(false);
        
        // Stop the audio stream if it's active
        m_audioManager->stopPlayback();
    }

    void GuiManager::updateAudioFileList()
    {
        m_filePicker->updateAudioFileList();
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
}