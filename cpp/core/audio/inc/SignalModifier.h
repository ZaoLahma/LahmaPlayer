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

        /**
        * @brief modify modifies the audio data
        * @param samples The audio data to modify
        * @param numSamples The number of samples to modify
        */
        virtual void modify(std::vector<float>& samples, uint32_t numSamples) const = 0;
    };
}