#pragma once

#include "AudioFile.h"
#define MINIMP3_FLOAT_OUTPUT
#include "minimp3_ex.h"

namespace LahmaPlayer::AudioFile
{
class AudioMp3File : public AudioFile
{
  public:
    AudioMp3File(const std::string &fileName);
    ~AudioMp3File();

    AudioFormat getAudioFormat() override;
    bool hasMore() const override;
    void read(std::vector<float> &samples, uint32_t numSamples) override;
    void seek(uint32_t numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection direction) override;
    uint32_t getTotalSamples() const override;

    static size_t skipID3Header(std::fstream &file);

  private:
    std::vector<uint8_t> m_mp3Data;
    mp3dec_ex_t m_decoder;
    mp3dec_frame_info_t m_frameInfo;
    uint32_t m_currentSample = 0;
    bool m_hasMore = true;
};
} // namespace LahmaPlayer::AudioFile
