#pragma once

#include "AudioSource.h"
#include "AudioSourceFactory.h"
#include "AudioStream.h"
#include "DspEngine.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <atomic>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace LahmaPlayer::Gui
{
class ControlsComponent
{
  public:
    ControlsComponent();
    ~ControlsComponent();

    /**
     * @brief Create the controls component
     * @return ftxui::Component representing the controls
     */
    ftxui::Component createComponent();

    /**
     * @brief Set the audio source for controls
     * @param audioSource The audio source to control
     */
    void setAudioSource(std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> audioSource);

    /**
     * @brief Set the audio stream for controls
     * @param audioStream The audio stream to control
     */
    void setAudioStream(std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> audioStream);

    /**
     * @brief Set the file name for controls
     * @param fileName The name of the file being controlled
     */
    void setFileName(const std::string &fileName);

    /**
     * @brief Set the play/pause state
     * @param isPlaying Whether audio is currently playing
     */
    void setIsPlaying(bool isPlaying);

    /**
     * @brief Get the play/pause state
     * @return true if audio is currently playing, false otherwise
     */
    bool getIsPlaying() const
    {
        return m_isPlaying;
    }

    /**
     * @brief Get the progress of audio playback
     * @return Current progress as a double between 0 and 1
     */
    double getProgress() const
    {
        return m_progress;
    }

    /**
     * @brief Set the progress of audio playback
     * @param progress Progress value between 0 and 1
     */
    void setProgress(double progress)
    {
        m_progress = progress;
    }

    /**
     * @brief Set the callback for play/pause operations
     * @param callback Function to call when play/pause is requested
     */
    void setPlayPauseCallback(std::function<void()> callback)
    {
        m_playPauseCallback = callback;
    }

    /**
     * @brief Set the callback for stop operations
     * @param callback Function to call when stop is requested
     */
    void setStopCallback(std::function<void()> callback)
    {
        m_stopCallback = callback;
    }

    /**
     * @brief Set the callback for exit operations
     * @param callback Function to call when exit is requested
     */
    void setExitCallback(std::function<void()> callback)
    {
        m_exitCallback = callback;
    }

    /**
     * @brief Set the callback for file selection
     * @param callback Function to call when a file is selected
     */
    void setFileSelectedCallback(std::function<void(const std::string &)> callback)
    {
        m_fileSelectedCallback = callback;
    }

    /**
     * @brief Set the callback for next track
     * @param callback Function to call when next is requested
     */
    void setNextCallback(std::function<void()> callback)
    {
        m_nextCallback = callback;
    }

    /**
     * @brief Set the callback for previous track
     * @param callback Function to call when previous is requested
     */
    void setPrevCallback(std::function<void()> callback)
    {
        m_prevCallback = callback;
    }

    /**
     * @brief Set the callback for reset playlist
     * @param callback Function to call when reset is requested
     */
    void setResetCallback(std::function<void()> callback)
    {
        m_resetCallback = callback;
    }

    /**
     * @brief Load an audio file
     * @param fileName The name of the file to load
     */
    void loadAudioFile(const std::string &fileName);

    /**
     * @brief Get the file name
     * @return Current file name
     */
    const std::string &getFileName() const
    {
        return m_fileName;
    }

  private:
    // Audio state
    std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> m_audioSource;
    std::shared_ptr<LahmaPlayer::DspEngine::DspEngine> m_dspEngine;
    std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> m_audioStream;
    std::string m_fileName;

    // UI state
    std::atomic<bool> m_isPlaying;
    std::atomic<double> m_progress;

    // Callbacks
    std::function<void()> m_playPauseCallback;
    std::function<void()> m_stopCallback;
    std::function<void()> m_exitCallback;
    std::function<void(const std::string &)> m_fileSelectedCallback;
    std::function<void()> m_nextCallback;
    std::function<void()> m_prevCallback;
    std::function<void()> m_resetCallback;
};
} // namespace LahmaPlayer::Gui