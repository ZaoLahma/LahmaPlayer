#include "AudioManager.h"
#include "GuiManager.h"
#include "AudioSourceFactory.h"
#include "HeadlessAudioManager.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    bool headless = false;
    std::string audioFile;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--headless" || arg == "-h") {
            headless = true;
        } else if (headless && arg[0] != '-') {
            audioFile = arg;
        }
    }

    if (headless) {
        std::cout << "Headless mode enabled" << std::endl;
        std::cout.flush();
        
        // Use HeadlessAudioManager - simple interface!
        LahmaPlayer::Headless::HeadlessAudioManager audioManager;
        
        if (!audioFile.empty()) {
            std::cout << "Playing: " << audioFile << std::endl;
            std::cout.flush();
            
            // Simple interface: just play the file!
            // playFile() handles loading, playing, and waiting for completion
            bool success = audioManager.playFile(audioFile);
            
            if (success) {
                std::cout << "Playback completed successfully" << std::endl;
            } else {
                std::cerr << "Error: Failed to play audio file" << std::endl;
            }
        } else {
            std::cerr << "No audio file specified" << std::endl;
            return 1;
        }
        
        return 0;
    }

    // Create GUI manager
    LahmaPlayer::Gui::GuiManager guiManager;

    // Start the GUI
    guiManager.startLoop();

    return 0;
}
