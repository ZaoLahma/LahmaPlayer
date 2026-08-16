#pragma once

#include "AudioManager.h"
#include "AudioSource.h"
#include "AudioSourceFactory.h"
#include "AudioStream.h"
#include "AudioControlsComponent.h"
#include "DspEngine.h"
#include "FilePickerComponent.h"
#include "Playlist.h"
#include "SystemControlsComponent.h"
#include "CurrentTrackDisplayComponent.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <atomic>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

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
    std::unique_ptr<AudioControlsComponent> m_controls;
    std::unique_ptr<SystemControlsComponent> m_systemControls;

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
    bool isAudioFile(const std::string &filename);

    // Playlist
    std::unique_ptr<Playlist> m_playlist;
    void playNextTrack();
    void playPreviousTrack();
    void resetPlaylist();
};
} // namespace LahmaPlayer::Gui