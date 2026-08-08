#include "AudioStream.h"
#include "AudioWavFile.h"
#include "DspEngine.h"
#include "SignalGainModifier.h"
#include "SignalSampleWidthModifier.h"
#include "GuiManager.h"

#include <iostream>
#include <string>
#include "AudioSourceFactory.h"
#include <thread>
#include <chrono>

using namespace ftxui;

int main(int argc, char *argv[])
{
    // Create GUI manager
    LahmaPlayer::Gui::GuiManager guiManager;

    // Start the GUI
    guiManager.startLoop();

    return 0;
}
