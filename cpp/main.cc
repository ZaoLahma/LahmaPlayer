#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "AudioManager.h"
#include "AudioSourceFactory.h"
#include "GuiManager.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    bool headless = false;
    std::string audioFileOrDir;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--headless" || arg == "-h")
        {
            headless = true;
        }
        else if (headless && arg[0] != '-')
        {
            audioFileOrDir = arg;
        }
    }

    // Initialize logger
    LahmaPlayer::Logger::getInstance().init("", true); // Headless: terminal output

    if (headless)
    {
        LahmaPlayer::Gui::AudioManager audioManager;

        if (!audioFileOrDir.empty())
        {
            std::cout << "Playing file: " << audioFileOrDir << std::endl;
            std::cout.flush();

            bool success = audioManager.loadAudioFile(audioFileOrDir);
            if (success)
            {
                audioManager.startPlayback();
                std::cout << "Playback started successfully. Press Ctrl+C to stop." << std::endl;
                std::cout.flush();

                // Keep running until interrupted
                while (true)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            else
            {
                std::cerr << "Error: Failed to load file" << std::endl;
            }
        }
        else
        {
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
