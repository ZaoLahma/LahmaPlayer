#include "GuiManager.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>

namespace LahmaPlayer::Gui
{
    GuiManager::GuiManager() : m_isPlaying(false), m_hasAudioFileLoaded(false), m_progress(0.0)
    {
        m_screen = std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
        LahmaPlayer::Logger::getInstance().init("gui.log", true);
        LahmaPlayer::Logger::getInstance().info("GuiManager initialized");
        
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
            LahmaPlayer::Logger::getInstance().info("File selected in callback: " + fileName);
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
        LahmaPlayer::Logger::getInstance().info("loadAudioFile called");
        LahmaPlayer::Logger::getInstance().info("Selected file index: " + std::to_string(m_filePicker->getSelectedFileIndex()));
        LahmaPlayer::Logger::getInstance().info("Audio files count: " + std::to_string(m_filePicker->getAudioFiles().size()));
        
        // Stop any current playback
        stopPlayback();
        
        // Check if a file is selected
        int selected_index = m_filePicker->getSelectedFileIndex();
        if (selected_index >= 0 && selected_index < static_cast<int>(m_filePicker->getAudioFiles().size()))
        {
            std::string fileName = m_filePicker->getAudioFiles()[selected_index];
            LahmaPlayer::Logger::getInstance().info("Selected file: " + fileName);
            
            // Load audio file through audio manager
            bool success = m_audioManager->loadAudioFile(fileName);
            
            if (!success)
            {
                LahmaPlayer::Logger::getInstance().warning("Failed to load audio file: " + fileName);
                return;
            }
            
            // Update controls with new audio source
            m_controls->setAudioSource(m_audioManager->getAudioSource());
            m_controls->setAudioStream(m_audioManager->getAudioStream());
            m_controls->setFileName(fileName);
            m_hasAudioFileLoaded = true;
            
            LahmaPlayer::Logger::getInstance().info("Audio file loaded successfully: " + fileName);
        }
        else
        {
            LahmaPlayer::Logger::getInstance().warning("No valid file selected");
        }
    }

    void GuiManager::startPlayback()
    {
        LahmaPlayer::Logger::getInstance().info("startPlayback called");
        if (!m_audioManager->getAudioStream())
        {
            std::cout << "No audio stream available" << std::endl;
            LahmaPlayer::Logger::getInstance().warning("No audio stream available");
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
        LahmaPlayer::Logger::getInstance().info("Playback started");
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

    bool GuiManager::isAudioFile(const std::string& filename)
    {
        // Use AudioSourceFactory to determine if the file is a supported audio file
        auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(filename);
        
        // Return true if we successfully created an audio source (not nullptr)
        return audioSource != nullptr;
    }
}