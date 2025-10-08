#include "AudioWavFile.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>

namespace LahmaPlayer
{
namespace AudioFile
{

    AudioWavFile::AudioFileFormat AudioWavFile::getAudioFileFormat()
    {
        m_file.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));
        
        AudioFileFormat format;
        format.numChannels = m_header.numChannels;
        format.sampleRate = m_header.sampleRate;
        format.bitsPerSample = m_header.bitsPerSample;

        return format;
    }

    bool AudioWavFile::hasMore()
    {
        return m_hasMore;
    }

    void AudioWavFile::read(std::vector<float>& samples, int numSamples)
    {
        samples.resize(numSamples);
        
        if (m_header.bitsPerSample == 32)
        {
            std::vector<uint32_t> raw(numSamples);
            m_file.read(reinterpret_cast<char*>(raw.data()), numSamples * sizeof(int32_t));
            for (int i = 0; i < numSamples; i++)
            {
                samples[i] = std::max(-1.0f, raw[i] / 2147483647.0f);
            }
        }
        else if (m_header.bitsPerSample == 16)
        {
            std::vector<int16_t> raw(numSamples);
            m_file.read(reinterpret_cast<char*>(raw.data()), numSamples * sizeof(int16_t));
            for (int i = 0; i < numSamples; i++)
            {
                samples[i] = std::max(-1.0f, raw[i] / 32767.0f);
            }
        }
        else if (m_header.bitsPerSample == 8)
        {
            std::vector<uint8_t> raw(numSamples);
            m_file.read(reinterpret_cast<char*>(raw.data()), numSamples);
            for (int i = 0; i < numSamples; i++)
            {
                samples[i] = std::max(-1.0f, raw[i] / 127.0f);
            }
        }

        m_hasMore = !m_file.eof();
    }
}
}