#pragma once

#include "AudioFile.h"

#include <memory>

namespace LahmaPlayer::AudioSource
{
    /**
    * @class AudioSourceFactory
    * @brief AudioSourceFactory is a factory class that creates AudioSource objects based on file contents
    */
    class AudioSourceFactory
    {
    public:
        static std::shared_ptr<AudioFile::AudioFile> createAudioSource(const std::string& fileName);

    private:
        static bool hasWavHeader(std::fstream& file);
        static bool hasMp3Header(std::fstream& file);
    };
}