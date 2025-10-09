#pragma once

#include "AudioSource.h"
#include "SignalModifier.h"
#include <memory>
namespace LahmaPlayer::DspEngine
{

    /**
    * @class DspEngine
    * @brief DspEngine is a class that is used to apply signal modifiers to an audio source
    */
    class DspEngine : public AudioSource::AudioSource
    {
    public:
        DspEngine(std::shared_ptr<AudioSource> audioSource) : m_audioSource(audioSource) {}
        ~DspEngine();

        void read(std::vector<float>& samples, uint32_t numSamples) override;
        bool hasMore() const override;
        AudioFormat getAudioFormat() override;

        void registerSignalModifier(std::shared_ptr<SignalModifier::SignalModifier> signalModifier);

    private:
        std::shared_ptr<AudioSource> m_audioSource;

        std::vector<std::shared_ptr<SignalModifier::SignalModifier>> m_signalModifiers;
    };
}