#include "AudioFile.h"

namespace LahmaPlayer::AudioFile
{

void AudioFile::seek(uint32_t numSamples, AudioSource::SeekDirection direction)
{
    // Base class provides no seek functionality
    // Derived classes implement their own seek logic
    // This is a no-op placeholder for completeness
}

} // namespace LahmaPlayer::AudioFile
