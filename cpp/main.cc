/*#include "AudioStream.h"
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
}*/


#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT
#include "minimp3.h"

#include <fstream>
#include <iostream>
#include <vector>

int main() {
    mp3dec_t dec;
    mp3dec_frame_info_t info;
    mp3dec_init(&dec);

    std::ifstream f("test.mp3", std::ios::binary);
    std::vector<uint8_t> buf(4096);
    std::vector<float> pcm(1152 * 2);

    while (f) {
        f.read(reinterpret_cast<char*>(buf.data()), buf.size());
        int bytes = static_cast<int>(f.gcount());
        if (!bytes) break;

        int samples = mp3dec_decode_frame(&dec, buf.data(), bytes, pcm.data(), &info);

        if (samples > 0) {
            std::cout << "Decoded " << samples << " samples @ " 
                      << info.hz << " Hz, " << info.channels << " ch\n";
            std::cout << "First few samples: "
                      << pcm[0] << ", " << pcm[1] << ", " << pcm[2] << "\n";
            break;
        }
    }
}