#pragma once

#include <cstdint>
#include <vector>
namespace LahmaPlayer::AudioSource
{
    /**
    * @class AudioSource
    * @brief AudioSource is a class that is used to read audio data
    */
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
        virtual bool hasMore() const = 0;
        virtual void read(std::vector<float>& samples, uint32_t numSamples) = 0;
    };
}