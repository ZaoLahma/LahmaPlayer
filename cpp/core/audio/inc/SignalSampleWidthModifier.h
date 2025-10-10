#pragma once

#include "SignalModifier.h"
#include <cstdint>
#include <vector>

namespace LahmaPlayer::SignalModifier
{
    class SignalSampleWidthModifier : public SignalModifier::SignalModifier
    {
    public:
        void setSampleBits(int bits);
        uint32_t requiredNumSamples(uint32_t numSamples) const override { return numSamples; }
        void modify(std::vector<float>& samples, uint32_t numSamples) const override;

    private:
        uint8_t m_sampleBits = 16;
    };
}