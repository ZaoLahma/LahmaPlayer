#include "DirectoryPickerComponent.h"
#include <AudioSourceFactory.h>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace LahmaPlayer::Gui
{
DirectoryPickerComponent::DirectoryPickerComponent()
    : m_currentDirectory("."), m_selectedDirectoryIndex(0)
{
    updateDirectoryList();
}

DirectoryPickerComponent::~DirectoryPickerComponent() {}

ftxui::Component DirectoryPickerComponent::createComponent()
{
    // Create left directory panel with menu options
    std::function<void()> on_enter_callback = [this]()
    {
        if (m_selectedDirectoryIndex >= 0 && m_selectedDirectoryIndex < static_cast<int>(m_directoryEntries.size()))
        {
            std::string entry = m_directoryEntries[m_selectedDirectoryIndex];
            
            // Handle ".." specially
            if (entry == ".." || entry == "../")
            {
                goUp();
            }
            else
            {
                enterDirectory(m_selectedDirectoryIndex);
            }
        }
    };
    
    auto menu_option = ftxui::MenuOption::Vertical();
    menu_option.on_enter = on_enter_callback;
    
    auto directory_menu = ftxui::Menu(&m_directoryEntries, &m_selectedDirectoryIndex, menu_option);
    
    auto directory_renderer = ftxui::Renderer(directory_menu,
        [this, directory_menu]
        {
            return ftxui::vbox({
                ftxui::text("Directories:") | ftxui::bold,
                ftxui::separator(), directory_menu->Render() | ftxui::frame
            });
        });

    // Create right audio files panel  
    auto audio_files_renderer = ftxui::Renderer([this]
    {
        std::vector<ftxui::Element> elements;
        elements.push_back(ftxui::text("Audio Files:") | ftxui::bold);
        elements.push_back(ftxui::separator());
        if (m_audioFiles.empty())
        {
            elements.push_back(ftxui::text("(No audio files)"));
        }
        else
        {
            std::string loadedFile = m_loadedFile;
            // Extract basename from loaded file path (e.g., "./test.mp3" -> "test.mp3")
            std::filesystem::path p(loadedFile);
            std::string loadedBasename = p.filename().string();
            for (const auto& file : m_audioFiles)
            {
                bool is_loaded = (file == loadedBasename);
                if (is_loaded)
                {
                    auto highlighted = ftxui::text("-> " + file) | ftxui::color(ftxui::Color::Green) | ftxui::bold;
                    elements.push_back(highlighted);
                }
                else
                {
                    elements.push_back(ftxui::text(file));
                }
            }
        }
        return ftxui::vbox(elements);
    });

    // Combine into horizontal layout
    return ftxui::Container::Horizontal({directory_renderer, audio_files_renderer});
}

void DirectoryPickerComponent::updateDirectoryList()
{
    m_directoryEntries.clear();
    m_audioFiles.clear();

    try
    {
        // Add ".." for parent directory at the beginning
        m_directoryEntries.push_back("..");
        
        std::vector<std::string> all_entries;
        for (const auto &entry : std::filesystem::directory_iterator(m_currentDirectory))
        {
            std::string name = entry.path().filename().string();
            if (entry.is_directory())
            {
                all_entries.push_back(name + "/");
            }
            else if (isSupportedAudioFile(entry.path()))
            {
                all_entries.push_back(name);
                std::cout << "DEBUG: Found audio file: " << name << std::endl;
            }
        }

        // Separate directories and audio files
        std::vector<std::string> directories;
        std::vector<std::string> audio;

        for (const auto &entry : all_entries)
        {
            if (entry.back() == '/')
            {
                directories.push_back(entry);
            }
            else
            {
                audio.push_back(entry);
            }
        }

        // Sort both lists alphabetically
        std::sort(directories.begin(), directories.end());
        std::sort(audio.begin(), audio.end());

        // Combine: ".." + sorted directories + audio files
        m_directoryEntries.clear();
        m_directoryEntries.insert(m_directoryEntries.end(), "..");
        m_directoryEntries.insert(m_directoryEntries.end(), directories.begin(), directories.end());
        m_audioFiles = audio;

        // Reset selected index
        if (m_selectedDirectoryIndex >= static_cast<int>(m_directoryEntries.size()))
        {
            m_selectedDirectoryIndex = 0;
        }

    }
    catch (const std::filesystem::filesystem_error &ex)
    {
        std::cerr << "Error reading directory: " << ex.what() << std::endl;
    }

    // Call refresh callback if set
    if (m_refreshCallback)
    {
        m_refreshCallback();
    }
}

void DirectoryPickerComponent::enterDirectory(int index)
{
    if (index >= 0 && index < static_cast<int>(m_directoryEntries.size()))
    {
        std::string entry = m_directoryEntries[index];
        
        // Skip ".." - it's handled separately
        if (entry == ".." || entry == "../")
        {
            return;
        }
        
        std::filesystem::path current(m_currentDirectory);
        std::string dir_name = entry;
        
        // Remove trailing slash if present
        if (!dir_name.empty() && dir_name.back() == '/')
        {
            dir_name = dir_name.substr(0, dir_name.length() - 1);
        }
        
        current /= dir_name;
        m_currentDirectory = current.string();
        m_selectedDirectoryIndex = 0;
        
        updateDirectoryList();

        // Call directory changed callback (for auto-loading first audio file)
        if (m_directoryChangedCallback)
        {
            m_directoryChangedCallback(m_currentDirectory);
        }
    }
}

void DirectoryPickerComponent::goUp()
{
    std::filesystem::path current(m_currentDirectory);
    std::string parent_path = current.parent_path().string();
    
    if (parent_path == ".")
    {
        m_currentDirectory = parent_path;
    }
    else
    {
        m_currentDirectory = parent_path;
    }
    
    updateDirectoryList();

    // Call directory changed callback
    if (m_directoryChangedCallback)
    {
        m_directoryChangedCallback(m_currentDirectory);
    }
}

bool DirectoryPickerComponent::isSupportedAudioFile(const std::filesystem::path &path) const
{
    // Use AudioSourceFactory to determine if the file is a supported audio file
    auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(path.string());

    // Return true if we successfully created an audio source (not nullptr)
    return audioSource != nullptr;
}
} // namespace LahmaPlayer::Gui
