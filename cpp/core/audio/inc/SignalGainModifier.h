#pragma once

#include "SignalModifier.h"

#include <cstdint>
#include <vector>

namespace LahmaPlayer::SignalModifier
{
    class SignalGainModifier : public SignalModifier
    {
    public:
        SignalGainModifier() : m_gain(1.0f) {}

        void setGain(float gain);
        uint32_t requiredNumSamples(uint32_t numSamples) const override { return numSamples; }
        void modify(std::vector<float>& samples) const override;

    private:
        float m_gain;
    };
}