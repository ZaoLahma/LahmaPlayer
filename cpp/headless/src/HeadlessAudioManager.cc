#include "HeadlessAudioManager.h"

namespace LahmaPlayer::Headless
{

HeadlessAudioManager::HeadlessAudioManager() : m_audioManager(std::make_unique<Gui::AudioManager>()) {}

HeadlessAudioManager::~HeadlessAudioManager()
{
    stop();
}

bool HeadlessAudioManager::playFile(const std::string &fileName)
{
    // Stop any current playback first
    stop();

    // Load the audio file
    bool success = m_audioManager->loadAudioFile(fileName);
    if (!success)
    {
        return false;
    }

    // Start playback
    m_audioManager->startPlayback();

    // Wait until playback is finished
    m_audioManager->getAudioStream()->waitUntilFinished();

    return true;
}

void HeadlessAudioManager::stop()
{
    if (m_audioManager)
    {
        m_audioManager->stopPlayback();
    }
}

void HeadlessAudioManager::seekTo(uint32_t samples)
{
    if (m_audioManager)
    {
        m_audioManager->seek(samples, LahmaPlayer::AudioSource::AudioSource::SeekDirection::Forward);
    }
}

void HeadlessAudioManager::seekForward(uint32_t numSamples)
{
    if (m_audioManager)
    {
        m_audioManager->seek(numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection::Forward);
    }
}

void HeadlessAudioManager::seekBackward(uint32_t numSamples)
{
    if (m_audioManager)
    {
        m_audioManager->seek(numSamples, LahmaPlayer::AudioSource::AudioSource::SeekDirection::Backward);
    }
}

} // namespace LahmaPlayer::Headless
