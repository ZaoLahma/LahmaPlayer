#include "AudioControlsComponent.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

namespace LahmaPlayer::Gui
{
AudioControlsComponent::AudioControlsComponent() : m_isPlaying(false), m_progress(0.0) {}

AudioControlsComponent::~AudioControlsComponent() {}

ftxui::Component AudioControlsComponent::createComponent()
{
    // Create the current track display (file name)
    auto display = m_currentTrackDisplay.createComponent();

    // Create audio controls buttons with ASCII symbols
    auto play_button = ftxui::Button("> Play",
                                      [this]
                                      {
                                          if (m_playPauseCallback)
                                          {
                                              m_playPauseCallback();
                                          }
                                      });

    auto stop_button = ftxui::Button("X Stop",
                                      [this]
                                      {
                                          if (m_stopCallback)
                                          {
                                              m_stopCallback();
                                          }
                                          m_audioSource.reset();
                                      });

    auto next_button = ftxui::Button("> Next",
                                      [this]
                                      {
                                          if (m_nextCallback)
                                          {
                                              m_nextCallback();
                                          }
                                      });

    auto prev_button = ftxui::Button("v Prev",
                                      [this]
                                      {
                                          if (m_prevCallback)
                                          {
                                              m_prevCallback();
                                          }
                                      });

    // Create a horizontal row of controls
    auto controls_row = ftxui::Container::Horizontal({
        prev_button,
        play_button,
        stop_button,
        next_button
    });

    // Create a vertical layout: display on top, controls row below
    auto controls_section = ftxui::Container::Vertical({
        display,
        controls_row
    });

    return controls_section;
}

void AudioControlsComponent::setAudioSource(std::shared_ptr<LahmaPlayer::AudioSource::AudioSource> audioSource)
{
    m_audioSource = audioSource;
}

void AudioControlsComponent::setAudioStream(std::shared_ptr<LahmaPlayer::AudioStream::AudioStream> audioStream)
{
    m_audioStream = audioStream;
}

void AudioControlsComponent::setFileName(const std::string &fileName)
{
    m_fileName = fileName;
    m_currentTrackDisplay.setFileName(fileName);
}

void AudioControlsComponent::setIsPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

void AudioControlsComponent::loadAudioFile(const std::string &fileName)
{
    m_fileName = fileName;
    m_currentTrackDisplay.setFileName(fileName);
    // Note: The actual audio loading is now handled by AudioManager
    // This method just updates the file name in the controls
}
} // namespace LahmaPlayer::Gui