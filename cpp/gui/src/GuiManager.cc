#include "GuiManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <filesystem>

namespace LahmaPlayer::Gui
{
    GuiManager::GuiManager() : m_running(false), m_isPlaying(false), m_progress(0.0), m_selectedFileIndex(0)
    {
        m_screen = std::unique_ptr<ftxui::ScreenInteractive>(new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen()));
        m_component = createMainComponent();
        updateAudioFileList();
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
        
        // Check if a file is selected
        if (m_selectedFileIndex >= 0 && m_selectedFileIndex < static_cast<int>(m_audioFiles.size()))
        {
            m_fileName = m_audioFiles[m_selectedFileIndex];
        }
        
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
        
        // Stop any currently playing audio
        stopPlayback();
        
        m_isPlaying = true;
        m_progress = 0.0;
        
        // Start the audio stream
        m_audioStream->start(m_audioSource);
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

    bool GuiManager::isAudioFile(const std::string& filename)
    {
        // Use AudioSourceFactory to determine if file is a supported audio file
        // This allows us to support any audio format that AudioSourceFactory can handle
        auto audio_source = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(filename);
        return audio_source != nullptr;
    }

    ftxui::Component GuiManager::createMainComponent()
    {
        auto file_picker = createFilePickerComponent();
        auto load_button = ftxui::Button("Load", [this] { loadAudioFile(); });
        auto play_button = ftxui::Button("Play", [this] { startPlayback(); });
        auto stop_button = ftxui::Button("Stop", [this] { stopPlayback(); });
        auto refresh_button = ftxui::Button("Refresh Files", [this] { updateAudioFileList(); });
        auto exit_button = ftxui::Button("Exit", [this] {
            stopLoop();
            // Force exit by creating an exit event
            m_screen->Exit();
        });
        
        auto main_container = ftxui::Container::Vertical({
            file_picker,
            load_button,
            play_button,
            stop_button,
            refresh_button,
            exit_button
        });
        
        return main_container;
    }

    ftxui::Component GuiManager::createFilePickerComponent()
    {
        // Create a menu for audio files
        auto menu = ftxui::Menu(&m_audioFiles, &m_selectedFileIndex);
        
        // Create a renderer for the menu that shows selected file
        auto renderer = ftxui::Renderer(menu, [this, menu] {
            auto selected_file = (m_selectedFileIndex >= 0 && m_selectedFileIndex < static_cast<int>(m_audioFiles.size()))
                                ? m_audioFiles[m_selectedFileIndex]
                                : "No file selected";
            
            return ftxui::vbox({
                ftxui::text("Select audio file:") | ftxui::bold,
                ftxui::separator(),
                menu->Render() | ftxui::frame,
                ftxui::text("Selected: " + selected_file)
            });
        });
        
        return renderer;
    }
}