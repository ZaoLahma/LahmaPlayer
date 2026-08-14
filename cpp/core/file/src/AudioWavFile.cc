#include "AudioWavFile.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <vector>

namespace LahmaPlayer
{
namespace AudioFile
{

AudioWavFile::AudioFormat AudioWavFile::getAudioFormat()
{
    if (!m_initialized)
    {
        m_file.read(reinterpret_cast<char *>(&m_header), sizeof(m_header));
        m_initialized = true;
    }

    AudioFormat format;
    format.numChannels = m_header.numChannels;
    format.sampleRate = m_header.sampleRate;
    format.bitsPerSample = m_header.bitsPerSample;

    return format;
}

bool AudioWavFile::hasMore() const
{
    return m_hasMore;
}

void AudioWavFile::read(std::vector<float> &samples, uint32_t numSamples)
{
    samples.resize(numSamples);

    if (m_header.bitsPerSample == 32)
    {
        std::vector<uint32_t> raw(numSamples);
        m_file.read(reinterpret_cast<char *>(raw.data()), numSamples * sizeof(int32_t));
        for (int i = 0; i < numSamples; i++)
        {
            samples[i] = std::max(-1.0f, raw[i] / 2147483647.0f);
        }
    }
    else if (m_header.bitsPerSample == 16)
    {
        std::vector<int16_t> raw(numSamples);
        m_file.read(reinterpret_cast<char *>(raw.data()), numSamples * sizeof(int16_t));
        for (int i = 0; i < numSamples; i++)
        {
            samples[i] = std::max(-1.0f, raw[i] / 32767.0f);
        }
    }
    else if (m_header.bitsPerSample == 8)
    {
        std::vector<uint8_t> raw(numSamples);
        m_file.read(reinterpret_cast<char *>(raw.data()), numSamples);
        for (int i = 0; i < numSamples; i++)
        {
            samples[i] = std::max(-1.0f, raw[i] / 127.0f);
        }
    }

    m_hasMore = !m_file.eof();
    m_currentSample += numSamples;
}

void AudioWavFile::seek(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction)
{
    int64_t currentSample = m_currentSample;
    int64_t targetSample = currentSample + 
        (direction == LahmaPlayer::AudioSource::AudioSource::SeekDirection::Forward ? numSamples : -numSamples);
    
    // Clamp to valid range
    if (targetSample < 0) targetSample = 0;
    if (targetSample > m_totalSamples) targetSample = m_totalSamples;
    
    // Calculate byte offset from current position
    int64_t byteOffset = (targetSample - currentSample) * 
                         (m_header.bitsPerSample / 8 * m_header.numChannels);
    
    // Seek from current position
    m_file.seekg(static_cast<int32_t>(byteOffset), std::ios::cur);
    
    m_currentSample = targetSample;
}

uint32_t AudioWavFile::getTotalSamples() const
{
    return m_totalSamples;
}

} // namespace AudioFile
} // namespace LahmaPlayer
