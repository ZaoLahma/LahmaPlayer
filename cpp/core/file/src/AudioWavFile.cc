#include "AudioWavFile.h"

namespace LahmaPlayer
{
namespace AudioFile
{

    bool AudioWavFile::verifyFormat()
    {
        return true;
    }

    bool AudioWavFile::hasMore()
    {
        return true;
    }

    void AudioWavFile::read(float (&samples), int numSamples)
    {

    }
}
} // namespace AudioFile} // namespace LahmaPlayer