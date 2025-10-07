#pragma once

namespace LahmaPlayer::AudioFile
{
    class AudioFile
    {
    public:
        AudioFile();
        ~AudioFile();

        virtual bool hasMore() = 0;
        virtual void read(float (&samples), int numSamples) = 0;
    };
}