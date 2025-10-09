#pragma once

#include "AudioSource.h"
#include <fstream>
#include <vector>

namespace LahmaPlayer::AudioFile
{
    class AudioFile : public AudioSource::AudioSource
    {
    public:
        AudioFile(const std::string& fileName) { m_file.open(fileName, std::ios::in | std::ios::binary); }
        ~AudioFile() { m_file.close(); }

        virtual AudioFormat getAudioFormat() = 0;
        virtual bool hasMore() const = 0;
        virtual void read(std::vector<float>& samples, uint32_t numSamples) = 0;

    protected:
        std::fstream m_file;
    };
}