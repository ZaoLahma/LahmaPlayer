#pragma once

#include "AudioFile.h"
#include <cstdint>

namespace LahmaPlayer::AudioFile
{
    class AudioWavFile : public AudioFile
    {
    public:
        AudioWavFile(const std::string& fileName) : AudioFile(fileName) {}
        ~AudioWavFile() {}

        AudioFormat getAudioFormat() override;
        bool hasMore() override;
        void read(std::vector<float>& samples, int numSamples) override;

    private:
        struct WavHeader
        {
            char riff[4];          // "RIFF"
            uint32_t fileSize;
            char wave[4];          // "WAVE"
            char fmt[4];           // "fmt "
            uint32_t fmtSize;      // 16 for PCM
            uint16_t audioFormat;  // 1 = PCM
            uint16_t numChannels;  // 1 or 2
            uint32_t sampleRate;   // 44100, etc.
            uint32_t byteRate;     // sampleRate * numChannels * bitsPerSample/8
            uint16_t blockAlign;   // numChannels * bitsPerSample/8
            uint16_t bitsPerSample; // 16, 24, etc.
            char data[4];          // "data"
            uint32_t dataSize;     // Size of audio data
        };

        WavHeader m_header;
        bool m_hasMore = true;
    };
}