#pragma once

#include <fstream>

namespace LahmaPlayer::AudioFile
{
    class AudioFile
    {
    public:
        AudioFile(const std::string& fileName) { m_file.open(fileName, std::ios::in | std::ios::binary); }
        ~AudioFile() { m_file.close(); }

        virtual bool verifyFormat() = 0;
        virtual bool hasMore() = 0;
        virtual void read(float (&samples), int numSamples) = 0;

    protected:
        std::fstream m_file;
    };
}