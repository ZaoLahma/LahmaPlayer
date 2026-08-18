#include "CurrentTrackDisplayComponent.h"
#include <algorithm>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>

namespace LahmaPlayer::Gui
{
static constexpr int kDisplayNameWidth = 40;

CurrentTrackDisplayComponent::CurrentTrackDisplayComponent()
    : m_fileName(""), m_fileNameCallback(nullptr), m_scrollOffset(0), m_screen(nullptr), m_scrollActive(false)
{
}

CurrentTrackDisplayComponent::~CurrentTrackDisplayComponent()
{
    stopScrolling();
}

void CurrentTrackDisplayComponent::setScreen(ftxui::ScreenInteractive *screen)
{
    m_screen = screen;
}

void CurrentTrackDisplayComponent::startScrolling()
{
    stopScrolling();
    m_scrollActive = true;
    m_scrollingThread = std::thread(
        [this]()
        {
            while (m_scrollActive.load())
            {
                std::string fileName;
                ftxui::ScreenInteractive *screen;
                {
                    fileName = m_fileName;
                    screen = m_screen;
                }
                if (!fileName.empty() && screen)
                {
                    int period = std::max(kDisplayNameWidth, static_cast<int>(fileName.length()));
                    screen->Post(
                        [this, screen, period]()
                        {
                            if (m_scrollActive.load())
                            {
                                m_scrollOffset = (m_scrollOffset + 1) % period;
                            }
                        });
                    screen->Post(ftxui::Event::Custom);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
}

void CurrentTrackDisplayComponent::stopScrolling()
{
    m_scrollActive.store(false);
    if (m_scrollingThread.joinable())
    {
        m_scrollingThread.join();
    }
}

ftxui::Component CurrentTrackDisplayComponent::createComponent()
{
    auto component = ftxui::Renderer(
        [this]
        {
            if (m_fileName.empty())
            {
                return ftxui::text("") | ftxui::color(ftxui::Color::White);
            }

            int len = static_cast<int>(m_fileName.length());
            int gap = std::max(kDisplayNameWidth - len, 0);
            int period = len + gap;

            std::string unit = m_fileName + std::string(static_cast<size_t>(gap), ' ');
            std::string display;
            size_t needed = static_cast<size_t>(period - 1 + kDisplayNameWidth);
            while (display.length() < needed)
            {
                display += unit;
            }

            size_t offset = static_cast<size_t>(m_scrollOffset % period);
            return ftxui::text(display.substr(offset, static_cast<size_t>(kDisplayNameWidth))) |
                   ftxui::color(ftxui::Color::White);
        });

    return component;
}

void CurrentTrackDisplayComponent::setFileName(const std::string &fileName)
{
    m_fileName = fileName;
    m_scrollOffset = 0;
    if (m_fileNameCallback)
    {
        m_fileNameCallback(fileName);
    }
}

void CurrentTrackDisplayComponent::setFileNameCallback(std::function<void(const std::string &)> callback)
{
    m_fileNameCallback = callback;
}
} // namespace LahmaPlayer::Gui
