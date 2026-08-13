#include "SignalSampleWidthModifier.h"
#include <algorithm>
#include <cmath>

namespace LahmaPlayer::SignalModifier
{
void SignalSampleWidthModifier::setSampleBits(int bits)
{
    m_sampleBits = std::clamp(bits, 1, 24);
}

void SignalSampleWidthModifier::modify(std::vector<float> &samples) const
{
    // Number of integer steps for signed audio
    const float maxInt = float((1 << (m_sampleBits - 1)) - 1);

    for (float &s : samples)
    {
        // Clamp input
        float x = std::clamp(s, -1.0f, 1.0f);

        // Scale to integer range
        float scaled = x * maxInt;

        // Quantize
        float quantized = std::round(scaled);

        // Scale back to float
        s = quantized / maxInt;
    }
}
} // namespace LahmaPlayer::SignalModifier