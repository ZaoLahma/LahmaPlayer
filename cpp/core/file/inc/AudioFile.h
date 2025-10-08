#pragma once

#include <fstream>
#include <vector>

namespace LahmaPlayer::AudioFile
{
    class AudioFile
    {
    public:
        struct AudioFileFormat
        {
            int numChannels;
            int sampleRate;
            int bitsPerSample;
        };

        AudioFile(const std::string& fileName) { m_file.open(fileName, std::ios::in | std::ios::binary); }
        ~AudioFile() { m_file.close(); }

        virtual AudioFileFormat getAudioFileFormat() = 0;
        virtual bool hasMore() = 0;
        virtual void read(std::vector<float>& samples, int numSamples) = 0;

    protected:
        std::fstream m_file;
    };
}