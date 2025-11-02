#pragma once

#include "AudioFile.h"
#define MINIMP3_FLOAT_OUTPUT
#include "minimp3.h"

namespace LahmaPlayer::AudioFile
{
    class AudioMp3File : public AudioFile
    {
    public:
        AudioMp3File(const std::string& fileName);
        ~AudioMp3File();

        AudioFormat getAudioFormat() override;
        bool hasMore() const override;
        void read(std::vector<float>& samples, uint32_t numSamples) override;
        void seek(uint32_t numSamples, AudioSource::SeekDirection direction) override;

        static size_t skipID3Header(std::fstream& file);

    private:
        mp3dec_t m_decoder;
        mp3dec_frame_info_t m_frameInfo;
        bool m_initialized = false;
        uint32_t m_currSampleNumber = 0;
    };
}