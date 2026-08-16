#include "GuiManager.h"
#include "Logger.h"
#include "SystemControlsComponent.h"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

namespace LahmaPlayer::Gui
{
GuiManager::GuiManager() : m_isPlaying(false), m_hasAudioFileLoaded(false), m_progress(0.0), m_playlist(nullptr)
{
    m_screen =
        std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
    LahmaPlayer::Logger::getInstance().init("gui.log", true);
    LahmaPlayer::Logger::getInstance().info("GuiManager initialized");

    // Initialize audio manager
    m_audioManager = std::make_unique<AudioManager>();

    // Initialize components
    m_directoryPicker = std::make_unique<DirectoryPickerComponent>();
    m_controls = std::make_unique<AudioControlsComponent>();
    m_systemControls = std::make_unique<SystemControlsComponent>();

    updateDirectoryList();

    // Initialize playlist with same files as directory picker
    if (!m_directoryPicker->getAudioFiles().empty())
    {
        std::string dirPath = ".";
        m_playlist = std::make_unique<Playlist>();
        m_playlist->loadFromDirectory(dirPath);
        
        // Auto-load first audio file on startup
        if (m_playlist->hasMore())
        {
            std::string firstFile = m_playlist->currentTrackFileName();
            LahmaPlayer::Logger::getInstance().info("Auto-loading first file: " + firstFile);
            m_audioManager->loadAudioFile(firstFile);
            if (m_audioManager->getAudioSource())
            {
                LahmaPlayer::Logger::getInstance().info("Audio file loaded successfully");
            }
        }
    }
    else
    {
        m_playlist = std::make_unique<Playlist>();
    }
}

GuiManager::~GuiManager()
{
    stopLoop();
}

void GuiManager::startLoop()
{
    // Wire up audio manager callback for track completion
    m_audioManager->setOnPlaybackFinishedCallback(
        [this]()
        {
            LahmaPlayer::Logger::getInstance().info("Track finished, auto-playing next");
            // Auto-play next track if available
            if (m_playlist && m_playlist->hasMore())
            {
                playNextTrack();
            }
        });

    // Set up callbacks for audio controls component
    m_controls->setPlayPauseCallback(
        [this]()
        {
            if (m_isPlaying)
            {
                stopPlayback();
            }
            else
            {
                if (!m_hasAudioFileLoaded)
                {
                    loadAudioFile();
                }
                startPlayback();
            }
        });

    m_controls->setStopCallback(
        [this]()
        {
            stopPlayback();
            m_hasAudioFileLoaded = false;
        });

    // Set up callbacks for controls component (for file selection)
    m_controls->setFileSelectedCallback(
        [this](const std::string &fileName)
        {
            LahmaPlayer::Logger::getInstance().info("File selected in callback: " + fileName);
            m_audioManager->loadAudioFile(fileName);
        });

    // Set up Next button callback
    m_controls->setNextCallback(
        [this]()
        {
            LahmaPlayer::Logger::getInstance().info("Next button clicked");
            playNextTrack();
        });

    // Set up Previous button callback
    m_controls->setPrevCallback(
        [this]()
        {
            LahmaPlayer::Logger::getInstance().info("Previous button clicked");
            playPreviousTrack();
        });

    // Set up Exit callback for system controls
    m_systemControls->setExitCallback(
        [this]()
        {
            stopLoop();
        });

    // Get the components
    auto directory_picker = m_directoryPicker->createComponent();
    auto controls = m_controls->createComponent();
    auto system_controls = m_systemControls->createComponent();

    // Set up callbacks for directory changes (for auto-loading first audio file)
    m_directoryPicker->setDirectoryChangedCallback(
        [this](const std::string &directory)
        {
            LahmaPlayer::Logger::getInstance().info("Directory changed: " + directory);
            // Auto-load first audio file if available
            if (!m_playlist || !m_playlist->hasMore())
            {
                LahmaPlayer::Logger::getInstance().info("No audio files in directory");
                return;
            }
            std::string firstFile = m_playlist->currentTrackFileName();
            LahmaPlayer::Logger::getInstance().info("Auto-loading: " + firstFile);
            m_audioManager->loadAudioFile(firstFile);
            if (m_audioManager->getAudioSource())
            {
                LahmaPlayer::Logger::getInstance().info("Audio file loaded, waiting for Play button");
            }
            else
            {
                LahmaPlayer::Logger::getInstance().warning("Failed to load audio file");
            }
        });

    // Set up callbacks for file name updates via controls component
    m_controls->getCurrentTrackDisplay().setFileNameCallback(
        [this](const std::string &fileName)
        {
            m_fileName = fileName;
        });

    // Create left column with directory picker and audio controls
    auto left_column = ftxui::Container::Vertical({directory_picker, controls});

    // Create right column with system controls
    auto right_column = ftxui::Container::Vertical({system_controls});

    // Create a horizontal layout with left column on the left and right column on the right
    auto main_container = ftxui::Container::Horizontal({left_column, right_column});

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
    LahmaPlayer::Logger::getInstance().info("Selected directory index: " +
                                            std::to_string(m_directoryPicker->getSelectedDirectoryIndex()));
    LahmaPlayer::Logger::getInstance().info("Audio files count: " +
                                            std::to_string(m_directoryPicker->getAudioFiles().size()));

    // Stop any current playback
    stopPlayback();

    // Check if a file is selected
    int selected_index = m_directoryPicker->getSelectedDirectoryIndex();
    if (selected_index >= 0 && selected_index < static_cast<int>(m_directoryPicker->getAudioFiles().size()))
    {
        std::string fileName = m_directoryPicker->getAudioFiles()[selected_index];
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
        m_controls->getCurrentTrackDisplay().setFileName(fileName);
        m_hasAudioFileLoaded = true;

        LahmaPlayer::Logger::getInstance().info("Audio file loaded successfully: " + fileName);
    }
    else
    {
        LahmaPlayer::Logger::getInstance().warning("No audio file selected");
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

void GuiManager::updateDirectoryList()
{
    m_directoryPicker->updateDirectoryList();
}

bool GuiManager::isAudioFile(const std::string &filename)
{
    // Use AudioSourceFactory to determine if the file is a supported audio file
    auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(filename);

    // Return true if we successfully created an audio source (not nullptr)
    return audioSource != nullptr;
}

void GuiManager::playNextTrack()
{
    LahmaPlayer::Logger::getInstance().info("playNextTrack called");

    if (!m_playlist)
    {
        LahmaPlayer::Logger::getInstance().warning("Playlist not initialized");
        return;
    }

    // Stop current track
    m_audioManager->stopPlayback();

    // Advance to next track (Playlist handles circular wrapping internally)
    if (m_playlist->hasMore())
    {
        m_playlist->advanceToNext();
    }
    else
    {
        LahmaPlayer::Logger::getInstance().warning("No tracks in playlist");
        return;
    }

    // Load and play next track
    std::string nextFile = m_playlist->currentTrackFileName();
    LahmaPlayer::Logger::getInstance().info("Loading next track: " + nextFile);

    bool success = m_audioManager->loadAudioFile(nextFile);
    if (success)
    {
        m_audioManager->startPlayback();
        m_hasAudioFileLoaded = true;
        m_controls->getCurrentTrackDisplay().setFileName(nextFile);
    }
    else
    {
        LahmaPlayer::Logger::getInstance().warning("Failed to load next track: " + nextFile);
    }
}

void GuiManager::playPreviousTrack()
{
    LahmaPlayer::Logger::getInstance().info("playPreviousTrack called");

    if (!m_playlist)
    {
        LahmaPlayer::Logger::getInstance().warning("Playlist not initialized");
        return;
    }

    // Stop current track
    m_audioManager->stopPlayback();

    // Advance to previous track (Playlist handles circular wrapping internally)
    if (m_playlist->hasMore())
    {
        m_playlist->advanceToPrevious();
    }
    else
    {
        LahmaPlayer::Logger::getInstance().warning("No tracks in playlist");
        return;
    }

    // Load and play previous track
    std::string prevFile = m_playlist->currentTrackFileName();
    LahmaPlayer::Logger::getInstance().info("Loading previous track: " + prevFile);

    bool success = m_audioManager->loadAudioFile(prevFile);
    if (success)
    {
        m_audioManager->startPlayback();
        m_hasAudioFileLoaded = true;
        m_controls->getCurrentTrackDisplay().setFileName(prevFile);
    }
    else
    {
        LahmaPlayer::Logger::getInstance().warning("Failed to load previous track: " + prevFile);
    }
}
} // namespace LahmaPlayer::Gui
