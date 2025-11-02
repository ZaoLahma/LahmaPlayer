#include "AudioSourceFactory.h"
#include "AudioMp3File.h"
#include "AudioWavFile.h"

#include <iostream>

namespace LahmaPlayer::AudioSource
{
    std::shared_ptr<AudioFile::AudioFile> AudioSourceFactory::createAudioSource(const std::string& fileName)
    {
        std::fstream file(fileName, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            std::cout<<"Failed to open file: "<<fileName<<std::endl;
            return nullptr;
        }
        if (hasWavHeader(file))
        {
            return std::make_shared<AudioFile::AudioWavFile>(fileName);
        }
        else if (hasMp3Header(file))
        {
            return std::make_shared<AudioFile::AudioMp3File>(fileName);
        }
        return nullptr;
    }

    bool AudioSourceFactory::hasWavHeader(std::fstream& file)
    {
        char header[4];
        file.read(header, 4);
        file.clear();
        file.seekg(0, std::ios::beg);
        return std::string(header, 4) == "RIFF";
    }

    bool AudioSourceFactory::hasMp3Header(std::fstream& file)
    {
        char header[2];
        file.seekg(AudioFile::AudioMp3File::skipID3Header(file), std::ios::beg);
        file.read(header, 2);
        file.clear();
        file.seekg(0, std::ios::beg);
        std::cout<<"MP3 Header bytes: "<<std::hex<<(static_cast<int>(header[0]) & 0xFF)<<" "<<(static_cast<int>(header[1]) & 0xFF)<<std::dec<<std::endl;
        return (static_cast<uint8_t>(header[0]) == 0xFF) && ((static_cast<uint8_t>(header[1]) & 0xE0) == 0xE0);
    }
}