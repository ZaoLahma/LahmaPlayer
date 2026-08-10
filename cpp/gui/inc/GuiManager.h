#pragma once

#include "AudioStream.h"
#include "AudioSource.h"
#include "DspEngine.h"
#include "AudioSourceFactory.h"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "AudioManager.h"
#include "FilePickerComponent.h"
#include "ControlsComponent.h"
#include <memory>
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

namespace LahmaPlayer::Gui
{
    /**
     * @class GuiManager
     * @brief Manages the FTXUI graphical user interface and its interaction with audio systems
     */
    class GuiManager
    {
    public:
        GuiManager();
        ~GuiManager();

        /**
         * @brief Start the GUI loop
         */
        void startLoop();

        /**
         * @brief Stop the GUI loop
         */
        void stopLoop();

        /**
         * @brief Load a new audio file for playback
         */
        void loadAudioFile();

        /**
         * @brief Start playback of the loaded audio file
         */
        void startPlayback();

        /**
         * @brief Stop playback of the loaded audio file
         */
        void stopPlayback();

    private:
        // GUI components
        std::unique_ptr<ftxui::ScreenInteractive> m_screen;
        ftxui::Component m_component;
        
        // Audio manager
        std::unique_ptr<AudioManager> m_audioManager;
        
        // UI components
        std::unique_ptr<FilePickerComponent> m_filePicker;
        std::unique_ptr<ControlsComponent> m_controls;
        
        // Audio state
        std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> m_audioSource;
        std::shared_ptr<LahmaPlayer::DspEngine::DspEngine> m_dspEngine;
        std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> m_audioStream;
        std::string m_fileName;
        
        // UI state
        std::atomic<bool> m_running;
        std::atomic<bool> m_isPlaying;
        std::atomic<bool> m_hasAudioFileLoaded;
        std::atomic<double> m_progress;
        
        // File picker state
        std::vector<std::string> m_audioFiles;
        int m_selectedFileIndex;
        
        // UI rendering
        ftxui::Component createMainComponent();
        ftxui::Component createFilePickerComponent();
        void updateAudioFileList();
        bool isAudioFile(const std::string& filename);
        
        // Logging
        void logToFile(const std::string& message);
        std::ofstream m_logFile;
    };
}