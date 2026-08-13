#include "ControlsComponent.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

namespace LahmaPlayer::Gui
{
ControlsComponent::ControlsComponent() : m_isPlaying(false), m_progress(0.0) {}

ControlsComponent::~ControlsComponent() {}

ftxui::Component ControlsComponent::createComponent()
{
    // Create buttons for controls
    auto play_button = ftxui::Button("Play",
                                     [this]
                                     {
                                         // Toggle play/pause using callback
                                         if (m_playPauseCallback)
                                         {
                                             m_playPauseCallback();
                                         }
                                     });

    auto stop_button = ftxui::Button("Stop",
                                     [this]
                                     {
                                         // Stop using callback
                                         if (m_stopCallback)
                                         {
                                             m_stopCallback();
                                         }
                                         // Clear the audio source reference
                                         m_audioSource.reset();
                                     });

    auto next_button = ftxui::Button("Next",
                                     [this]
                                     {
                                         if (m_nextCallback)
                                         {
                                             m_nextCallback();
                                         }
                                     });

    auto prev_button = ftxui::Button("Prev",
                                     [this]
                                     {
                                         if (m_prevCallback)
                                         {
                                             m_prevCallback();
                                         }
                                     });

    auto reset_button = ftxui::Button("Reset",
                                      [this]
                                      {
                                          if (m_resetCallback)
                                          {
                                              m_resetCallback();
                                          }
                                      });

    auto refresh_button = ftxui::Button("Refresh Files",
                                        [this]
                                        {
                                            // This will be handled by the main GuiManager
                                        });

    auto exit_button = ftxui::Button("Exit",
                                     [this]
                                     {
                                         // Stop playback and exit using callback
                                         if (m_stopCallback)
                                         {
                                             m_stopCallback();
                                         }
                                         // Call exit callback if set
                                         if (m_exitCallback)
                                         {
                                             m_exitCallback();
                                         }
                                     });

    // Create the controls component
    auto controls = ftxui::Container::Vertical(
        {play_button, stop_button, next_button, prev_button, reset_button, refresh_button, exit_button});

    return ftxui::Renderer(controls,
                           [controls]
                           {
                               return ftxui::vbox({ftxui::text("Controls:") | ftxui::bold, ftxui::separator(),
                                                   controls->Render() | ftxui::frame});
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

void ControlsComponent::setFileName(const std::string &fileName)
{
    m_fileName = fileName;
}

void ControlsComponent::setIsPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

void ControlsComponent::loadAudioFile(const std::string &fileName)
{
    m_fileName = fileName;
    // Note: The actual audio loading is now handled by AudioManager
    // This method just updates the file name in the controls
}
} // namespace LahmaPlayer::Gui