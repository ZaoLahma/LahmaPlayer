#pragma once

#include "AudioSourceFactory.h"
#include "ftxui/component/component.hpp"
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace LahmaPlayer::Gui
{
/**
 * @class DirectoryPickerComponent
 * @brief Split-panel directory browser: left for directories, right for audio files
 * 
 * This component provides a two-panel file browser:
 * - Left panel: Shows directories and parent directory (..), navigable with ↑/↓, Enter to enter
 * - Right panel: Shows audio files in current directory, read-only display
 * 
 * Auto-loads the first audio file when entering a directory.
 */
class DirectoryPickerComponent
{
  public:
    DirectoryPickerComponent();
    ~DirectoryPickerComponent();

    /**
     * @brief Create the directory picker component
     * @return ftxui::Component representing the two-panel directory picker
     */
    ftxui::Component createComponent();

    /**
     * @brief Update the directory and audio file lists
     */
    void updateDirectoryList();

    /**
     * @brief Set the callback for directory changes
     * @param callback Function to call when directory changes (for auto-loading first audio file)
     */
    void setDirectoryChangedCallback(std::function<void(std::string directory)> callback)
    {
        m_directoryChangedCallback = callback;
    }

    /**
     * @brief Set the callback for refresh operations
     * @param callback Function to call when refresh is requested
     */
    void setRefreshCallback(std::function<void()> callback)
    {
        m_refreshCallback = callback;
    }

    /**
     * @brief Set the currently loaded file
     * @param fileName The name of the currently loaded file
     */
    void setLoadedFile(const std::string &fileName)
    {
        m_loadedFile = fileName;
    }

    /**
     * @brief Get the currently loaded file
     * @return The name of the currently loaded file (empty if none)
     */
    const std::string &getLoadedFile() const
    {
        return m_loadedFile;
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
     * @brief Get the current directory path
     * @return Current directory path as string
     */
    std::string getCurrentDirectory() const
    {
        return m_currentDirectory;
    }

    /**
     * @brief Get the list of audio files in current directory
     * @return Vector of audio file paths
     */
    const std::vector<std::string> &getAudioFiles() const
    {
        return m_audioFiles;
    }

    /**
     * @brief Get the selected directory index
     * @return Index of currently selected directory
     */
    int getSelectedDirectoryIndex() const
    {
        return m_selectedDirectoryIndex;
    }

    /**
     * @brief Set the selected directory index
     * @param index Index of directory to select
     */
    void setSelectedDirectoryIndex(int index)
    {
        m_selectedDirectoryIndex = index;
    }

  private:
    /**
     * @brief Render the left directory panel
     * @return FTXUI element for directory panel
     */
    ftxui::Element renderDirectoryPanel();

    /**
     * @brief Render the right audio files panel
     * @return FTXUI element for audio files panel
     */
    ftxui::Element renderAudioFilesPanel();

    /**
     * @brief Enter a directory and update the lists
     * @param index Index of directory to enter
     */
    void enterDirectory(int index);

    /**
     * @brief Go to parent directory
     */
    void goUp();

    /**
     * @brief Check if a file is a supported audio file
     * @param path Full file path
     * @return true if file is a supported audio file
     */
    bool isSupportedAudioFile(const std::filesystem::path &path) const;

    // Current directory path
    std::string m_currentDirectory;

    // Directory entries (directories and "..")
    std::vector<std::string> m_directoryEntries;
    int m_selectedDirectoryIndex;

    // Audio files in current directory
    std::vector<std::string> m_audioFiles;
    std::string m_loadedFile;

    // Callbacks
    std::function<void(std::string directory)> m_directoryChangedCallback;
    std::function<void()> m_refreshCallback;
    std::function<void()> m_exitCallback;
};
} // namespace LahmaPlayer::Gui
