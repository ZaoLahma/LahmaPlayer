#pragma once

#include "AudioFile.h"

namespace LahmaPlayer::AudioFile
{
    class AudioWavFile : public AudioFile
    {
    public:
        AudioWavFile(const std::string& fileName) : AudioFile(fileName) {}
        ~AudioWavFile() {}

        bool verifyFormat() override;
        bool hasMore() override;
        void read(float (&samples), int numSamples) override;
    };
}