#include "FilePickerComponent.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <AudioSourceFactory.h>  // Include for AudioSource

namespace LahmaPlayer::Gui
{
    FilePickerComponent::FilePickerComponent() : m_selectedFileIndex(0)
    {
        updateAudioFileList();
    }

    FilePickerComponent::~FilePickerComponent()
    {
    }

    ftxui::Component FilePickerComponent::createComponent()
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

        // Create the main component container
        auto main_container = ftxui::Container::Vertical({
            renderer
        });
        
        return main_container;
    }

    void FilePickerComponent::updateAudioFileList()
    {
        m_audioFiles.clear();
        
        // Look for audio files in the current directory
        try {
            for (const auto& entry : std::filesystem::directory_iterator(".")) {
                if (entry.is_regular_file() && isAudioFile(entry.path().filename().string())) {
                    m_audioFiles.push_back(entry.path().filename().string());
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
        }
        
        // Sort the audio files alphabetically
        std::sort(m_audioFiles.begin(), m_audioFiles.end());
        
        // Reset selected index if needed
        if (m_selectedFileIndex >= static_cast<int>(m_audioFiles.size())) {
            m_selectedFileIndex = 0;
        }
    }

    bool FilePickerComponent::isAudioFile(const std::string& filename)
    {
        // Use AudioSourceFactory to determine if the file is a supported audio file
        auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(filename);
        
        // Return true if we successfully created an audio source (not nullptr)
        return audioSource != nullptr;
    }
}