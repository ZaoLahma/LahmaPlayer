#pragma once

#include "AudioFile.h"
#include <cstdint>

namespace LahmaPlayer::AudioFile
{
class AudioWavFile : public AudioFile
{
  public:
    AudioWavFile(const std::string &fileName) : AudioFile(fileName)
    {
        m_file.seekg(0, std::ios::end);
        size_t fileSize = m_file.tellg();
        m_file.seekg(0, std::ios::beg);
        
        m_file.read(reinterpret_cast<char *>(&m_header), sizeof(m_header));
        m_initialized = true;
        m_totalSamples = m_header.dataSize * m_header.numChannels / m_header.bitsPerSample;
    }
    ~AudioWavFile() {}

    AudioFormat getAudioFormat() override;
    bool hasMore() const override;
    void read(std::vector<float> &samples, uint32_t numSamples) override;
    void seek(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction) override;
    uint32_t getTotalSamples() const override;

  private:
    struct WavHeader
    {
        char riff[4]; // "RIFF"
        uint32_t fileSize;
        char wave[4];           // "WAVE"
        char fmt[4];            // "fmt "
        uint32_t fmtSize;       // 16 for PCM
        uint16_t audioFormat;   // 1 = PCM
        uint16_t numChannels;   // 1 or 2
        uint32_t sampleRate;    // 44100, etc.
        uint32_t byteRate;      // sampleRate * numChannels * bitsPerSample/8
        uint16_t blockAlign;    // numChannels * bitsPerSample/8
        uint16_t bitsPerSample; // 16, 24, etc.
        char data[4];           // "data"
        uint32_t dataSize;      // Size of audio data
    };

    bool m_initialized = false;
    WavHeader m_header;
    bool m_hasMore = true;
    uint32_t m_currentSample = 0;
    uint32_t m_totalSamples = 0;
};
} // namespace LahmaPlayer::AudioFile
