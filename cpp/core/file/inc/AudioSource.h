#pragma once

#include <vector>
namespace LahmaPlayer::AudioSource
{
    class AudioSource
    {
    public:
        struct AudioFormat
        {
            int numChannels;
            int sampleRate;
            int bitsPerSample;
        };

        virtual ~AudioSource() {}

        virtual AudioFormat getAudioFormat() = 0;
        virtual bool hasMore() = 0;
        virtual void read(std::vector<float>& samples, int numSamples) = 0;
    };
}