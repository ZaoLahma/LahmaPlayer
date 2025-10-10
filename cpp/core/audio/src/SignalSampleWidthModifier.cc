#include "SignalSampleWidthModifier.h"
#include <algorithm>
#include <cmath>

namespace LahmaPlayer::SignalModifier
{
    void SignalSampleWidthModifier::setSampleBits(int bits)
    {
        m_sampleBits = std::clamp(bits, 1, 16); 
    }

    void SignalSampleWidthModifier::modify(std::vector<float>& samples, uint32_t numSamples) const
    {
        float levels = (1 << m_sampleBits);  // 2^bits levels
        
        for (size_t i = 0; i < numSamples; i++)
        {
            // 1. Scale to integer range
            float scaled = samples[i] * levels;
            
            // 2. Quantize (round to nearest level)
            float quantized = round(scaled);
            
            // 3. Scale back to [-1, 1]
            samples[i] = quantized / levels;
        }
    }
}