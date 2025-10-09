#include "DspEngine.h"

namespace LahmaPlayer::DspEngine
{
    DspEngine::~DspEngine() {}

    void DspEngine::read(std::vector<float>& samples, uint32_t numSamples) 
    {
        uint32_t largestRequiredNumSamples = numSamples;

        for (uint32_t i = 0; i < m_signalModifiers.size(); i++)
        {
            largestRequiredNumSamples = std::max(largestRequiredNumSamples, m_signalModifiers[i]->requiredNumSamples(numSamples));
        }

        m_audioSource->read(samples, largestRequiredNumSamples);

        for (const auto& signalModifier : m_signalModifiers)
        {
            signalModifier->modify(samples);
        }
    }

    bool DspEngine::hasMore() const
    {
        return m_audioSource->hasMore(); 
    }

    AudioSource::AudioSource::AudioFormat DspEngine::getAudioFormat()
    {
        return m_audioSource->getAudioFormat();
    }

    void DspEngine::registerSignalModifier(std::shared_ptr<SignalModifier::SignalModifier> signalModifier)
    {
        m_signalModifiers.push_back(signalModifier);
    }
}