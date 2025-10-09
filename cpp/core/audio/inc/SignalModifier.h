#pragma once

#include <cstdint>
#include <vector>
namespace LahmaPlayer::SignalModifier
{
    /**
    * @class SignalModifier
    * @brief SignalModifier is a class that is used to modify audio data
    */
    class SignalModifier
    {
    public:
        virtual uint32_t requiredNumSamples(uint32_t numSamples) const = 0;
        virtual void modify(std::vector<float>& samples) const = 0;
    };
}