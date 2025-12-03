#include "AudioMp3File.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"
#include <algorithm>
#include <cstring>
#include <iostream>

constexpr int MAX_MP3_SAMPLES_PER_FRAME = 1152 * 2;

namespace LahmaPlayer
{
namespace AudioFile
{
    AudioMp3File::AudioMp3File(const std::string& fileName) : AudioFile(fileName)
    {
        // Load file into memory because that's apparently how minimp3_ex wants it
        // And hey, it's not like MP3 files are that big usually
        // (The embedded developer inside of me is crying a bit right now though)
        m_file.seekg(0, std::ios::end);
        size_t fileSize = m_file.tellg();
        m_file.seekg(0);

        m_mp3Data.resize(fileSize);
        m_file.read(reinterpret_cast<char*>(m_mp3Data.data()), fileSize);

        // Open decoder on memory buffer
        if (mp3dec_ex_open_buf(&m_decoder, m_mp3Data.data(), fileSize, MP3D_SEEK_TO_SAMPLE) != 0)
        {
            throw std::runtime_error("Failed to decode MP3 buffer");
        }
    }

    AudioMp3File::~AudioMp3File() {}

    AudioMp3File::AudioFormat AudioMp3File::getAudioFormat()
    {
        AudioFormat format;
        format.numChannels = m_decoder.info.channels;
        format.sampleRate = m_decoder.info.hz;
        format.bitsPerSample = 32;

        return format;
    }

    bool AudioMp3File::hasMore() const
    {
        return m_hasMore;
    }

    void AudioMp3File::read(std::vector<float>& samples, uint32_t numSamples)
    {
        samples.resize(numSamples);

        size_t samplesRead = mp3dec_ex_read(&m_decoder, samples.data(), numSamples);

        if (0 == samplesRead)
        {
            m_hasMore = false;
        }

        if (samplesRead < numSamples)
        {
            std::fill(samples.begin() + samplesRead, samples.end(), 0.0f);
        }
    }

    void AudioMp3File::seek(uint32_t numSamples, AudioSource::SeekDirection direction)
    {
    }

    size_t AudioMp3File::skipID3Header(std::fstream& file)
    {
        char header[10];
        file.read(header, 10);
        if (std::string(header, 3) != "ID3")
        {
            file.clear();
            file.seekg(0, std::ios::beg);
            return 0;
        }

        size_t size = (static_cast<size_t>(header[6] & 0x7F) << 21) |
                      (static_cast<size_t>(header[7] & 0x7F) << 14) |
                      (static_cast<size_t>(header[8] & 0x7F) << 7)  |
                      (static_cast<size_t>(header[9] & 0x7F));

        file.clear();
        file.seekg(10 + size, std::ios::beg);
        return 10 + size;
    }

}
}