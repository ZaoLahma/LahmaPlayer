#include "DspEngine.h"

#include <iostream>

namespace LahmaPlayer::DspEngine
{
    DspEngine::~DspEngine() {}

    AudioSource::AudioSource::AudioFormat DspEngine::getAudioFormat()
    {
        return m_audioSource->getAudioFormat();
    }

    bool DspEngine::hasMore() const
    {
        return m_audioSource->hasMore(); 
    }

    void DspEngine::read(std::vector<float>& samples, uint32_t numSamples) 
    {
        std::vector<size_t> inNeeded(m_signalModifiers.size() + 1);

        // Check how many samples each "layer" needs, starting with the absolute minimum amount
        // as given by numSamples
        inNeeded.back() = numSamples;
        for (int32_t i = m_signalModifiers.size() - 1; i >= 0; --i)
        {
            inNeeded[i] = m_signalModifiers[i]->requiredNumSamples(inNeeded[i + 1]);
        }

        samples.resize(inNeeded[0]);
        m_audioSource->read(samples, inNeeded[0]);

        for (uint32_t i = 0; i < m_signalModifiers.size(); i++)
        {
            m_signalModifiers[i]->modify(samples);
        }
    }

    void DspEngine::seek(uint32_t numSamples, AudioSource::SeekDirection direction)
    {
        m_audioSource->seek(numSamples, direction);
    }

    void DspEngine::registerSignalModifier(std::shared_ptr<SignalModifier::SignalModifier> signalModifier)
    {
        m_signalModifiers.push_back(signalModifier);
    }
}