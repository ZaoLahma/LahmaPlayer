#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <functional>

namespace LahmaPlayer::Gui
{
    class FilePickerComponent
    {
    public:
        FilePickerComponent();
        ~FilePickerComponent();

        /**
         * @brief Create the file picker component
         * @return ftxui::Component representing the file picker
         */
        ftxui::Component createComponent();

        /**
         * @brief Update the list of audio files
         */
        void updateAudioFileList();

        /**
         * @brief Check if a file is an audio file
         * @param filename The filename to check
         * @return true if the file is an audio file, false otherwise
         */
        bool isAudioFile(const std::string& filename);

        // Getters
        const std::vector<std::string>& getAudioFiles() const { return m_audioFiles; }
        int getSelectedFileIndex() const { return m_selectedFileIndex; }
        void setSelectedFileIndex(int index) { m_selectedFileIndex = index; }
        std::string getSelectedFileName() const { return m_audioFiles.empty() ? "" : m_audioFiles[m_selectedFileIndex]; }



        /**
         * @brief Set the callback for refresh operations
         * @param callback Function to call when refresh is requested
         */
        void setRefreshCallback(std::function<void()> callback) { m_refreshCallback = callback; }

        /**
         * @brief Set the callback for exit operations
         * @param callback Function to call when exit is requested
         */
        void setExitCallback(std::function<void()> callback) { m_exitCallback = callback; }

    private:
        // Audio files list
        std::vector<std::string> m_audioFiles;
        int m_selectedFileIndex;
        
        // Callbacks
        std::function<void()> m_refreshCallback;
        std::function<void()> m_exitCallback;
        
        // Logging
        void logToFile(const std::string& message);
        std::ofstream m_logFile;
    };
}