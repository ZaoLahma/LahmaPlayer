#include "CurrentTrackDisplayComponent.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <algorithm>
#include <chrono>
#include <thread>

namespace LahmaPlayer::Gui
{
CurrentTrackDisplayComponent::CurrentTrackDisplayComponent() : m_fileName(""), m_fileNameCallback(nullptr), m_scrollOffset(0), m_screen(nullptr), m_scrollActive(false) {}

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
    m_scrollingThread = std::thread([this]()
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
                // Post to UI thread - this lambda executes on the FTXUI main loop thread
                screen->Post([this, screen]()
                {
                    if (m_scrollActive.load())
                    {
                        m_scrollOffset++;
                        screen->RequestAnimationFrame();
                    }
                });
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
  // Use Renderer with capture by value to avoid capture issues and allow updates
  auto component = ftxui::Renderer([this]
  {
      if (m_fileName.empty())
      {
          return ftxui::text("") | ftxui::color(ftxui::Color::White);
      }

      // Use a fixed width of 40 characters for scrolling
      int screenWidth = 40;

      // Build a scrolling marquee: filename + large gap of spaces + filename + ...
      std::string display = m_fileName;
      std::string padding(screenWidth, ' ');
      while (display.length() < static_cast<unsigned>(screenWidth * 3))
      {
          display += padding + m_fileName;
      }

      // Scroll through the display string
      int start = m_scrollOffset % static_cast<int>(display.length());
      std::string scrolled = display.substr(start, screenWidth);

      return ftxui::text(scrolled) | ftxui::color(ftxui::Color::White);
  });

  return component;
}

void CurrentTrackDisplayComponent::setFileName(const std::string &fileName)
{
  m_fileName = fileName;
  // The Renderer will automatically update with the new file name on next render
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
