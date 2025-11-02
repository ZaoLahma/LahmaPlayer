#include "AudioMp3File.h"

#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include <algorithm>
#include <cstring>

constexpr int MAX_MP3_SAMPLES_PER_FRAME = 1152 * 2;

namespace LahmaPlayer
{
namespace AudioFile
{
    AudioMp3File::AudioMp3File(const std::string& fileName) : AudioFile(fileName)
    {
        mp3dec_init(&m_decoder);
    }

    AudioMp3File::~AudioMp3File() {}

    AudioMp3File::AudioFormat AudioMp3File::getAudioFormat()
    {
        if (!m_initialized)
        {
            uint8_t buffer[4096];
            AudioMp3File::skipID3Header(m_file);
            m_file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
            int bytesRead = static_cast<int>(m_file.gcount());
            int samplesDecoded = mp3dec_decode_frame(&m_decoder, buffer, bytesRead, nullptr, &m_frameInfo);
            m_initialized = true;
            m_file.clear();
            m_file.seekg(0, std::ios::beg);
            AudioMp3File::skipID3Header(m_file);
        }

        AudioFormat format;
        format.numChannels = m_frameInfo.channels;
        format.sampleRate = m_frameInfo.hz;
        format.bitsPerSample = 16; // minimp3 outputs 16-bit samples

        return format;
    }

    bool AudioMp3File::hasMore() const
    {
        return !m_file.eof();
    }

    void AudioMp3File::read(std::vector<float>& samples, uint32_t numSamples)
    {
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