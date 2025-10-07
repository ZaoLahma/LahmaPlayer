#pragma once

#include "AudioFile.h"

namespace LahmaPlayer::AudioFile
{
    class AudioWavFile : public AudioFile
    {
    public:
        AudioWavFile();
        ~AudioWavFile();

        bool hasMore() override;
        void read(float (&samples), int numSamples) override;
    };
}