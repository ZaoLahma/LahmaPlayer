#include "AudioStream.h"
#include "AudioWavFile.h"
#include "DspEngine.h"
#include "SignalGainModifier.h"
#include "SignalSampleWidthModifier.h"

#include <iostream>
#include <string>
#include "AudioSourceFactory.h"

int main(int argc, char *argv[])
{
    // Extract file name to open from the provided arguments
    if (argc < 2)
    {
        std::cout<<"Please provide a file name."<<std::endl;
        return 1;
    }
    const std::string fileName = argv[1];
    std::cout<<"Opening file: "<<fileName<<std::endl;

    auto audioSource = LahmaPlayer::AudioSource::AudioSourceFactory::createAudioSource(fileName);
    if (!audioSource)
    {
        std::cout<<"Failed to create audio source for file: "<<fileName<<std::endl;
        return 1;
    }
    auto dspEngine = std::make_shared<LahmaPlayer::DspEngine::DspEngine>(audioSource);

    auto gainModifier = std::make_shared<LahmaPlayer::SignalModifier::SignalGainModifier>();
    gainModifier->setGain(1.0f);
    auto sampleWidthModifier = std::make_shared<LahmaPlayer::SignalModifier::SignalSampleWidthModifier>();
    sampleWidthModifier->setSampleBits(16);

    dspEngine->registerSignalModifier(gainModifier);
    dspEngine->registerSignalModifier(sampleWidthModifier);

    LahmaPlayer::AudioStream::AudioStream stream;
    stream.start(dspEngine);

    stream.waitUntilFinished();

    return 0;
}
