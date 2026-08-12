#include "AudioManager.h"
#include "GuiManager.h"
#include "AudioSourceFactory.h"

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
        
        // Use AudioManager directly
        LahmaPlayer::Gui::AudioManager audioManager;
        
        if (!audioFile.empty()) {
            std::cout << "Loading file: " << audioFile << std::endl;
            std::cout.flush();
            
            bool success = audioManager.loadAudioFile(audioFile);
            if (success) {
                std::cout << "Starting playback..." << std::endl;
                std::cout.flush();
                
                audioManager.startPlayback();
                
                std::cout << "Playback started successfully. Press Ctrl+C to stop." << std::endl;
                std::cout.flush();
                
                // Keep running until interrupted
                while (true) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            } else {
                std::cerr << "Error: Failed to load audio file" << std::endl;
            }
        } else {
            std::cerr << "No audio file specified" << std::endl;
        }
        
        return 0;
    }

    // Create GUI manager
    LahmaPlayer::Gui::GuiManager guiManager;

    // Start the GUI
    guiManager.startLoop();

    return 0;
}
