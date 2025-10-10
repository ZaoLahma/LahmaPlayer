#include "SignalGainModifier.h"
#include <algorithm>
#include <sys/types.h>

namespace LahmaPlayer::SignalModifier
{
    void SignalGainModifier::setGain(float gain)
    {
        m_gain = std::clamp(gain, 0.0f, 2.0f);
    }

    void SignalGainModifier::modify(std::vector<float>& samples, uint32_t numSamples) const
    {
        for (int i = 0; i < numSamples; i++)
        {
            samples[i] *= m_gain;
        }
    }
}