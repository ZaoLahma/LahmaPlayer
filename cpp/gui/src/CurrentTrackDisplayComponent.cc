#include "CurrentTrackDisplayComponent.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace LahmaPlayer::Gui
{
CurrentTrackDisplayComponent::CurrentTrackDisplayComponent() : m_fileName(""), m_fileNameCallback(nullptr) {}

CurrentTrackDisplayComponent::~CurrentTrackDisplayComponent() {}

ftxui::Component CurrentTrackDisplayComponent::createComponent()
{
  // Use Renderer with capture by value to avoid capture issues and allow updates
  auto component = ftxui::Renderer([this]
  {
      return ftxui::text(m_fileName) | ftxui::color(ftxui::Color::White);
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
