#pragma once

#include "ftxui/component/screen_interactive.hpp"
#include <atomic>
#include <functional>
#include <string>
#include <thread>

namespace LahmaPlayer::Gui
{
class CurrentTrackDisplayComponent
{
public:
  CurrentTrackDisplayComponent();
  ~CurrentTrackDisplayComponent();

  /**
   * @brief Create the display component
   * @return ftxui::Component representing the file name display
   */
  ftxui::Component createComponent();

  /**
   * @brief Set the file name to display
   * @param fileName The name of the current audio file
   */
  void setFileName(const std::string &fileName);

  /**
   * @brief Set the callback for file name updates
   * @param callback Function to call when file name changes
   */
  void setFileNameCallback(std::function<void(const std::string &)> callback);

  /**
   * @brief Set the screen reference for forced redraws
   */
  void setScreen(ftxui::ScreenInteractive *screen);

  /**
   * @brief Start scrolling animation
   */
  void startScrolling();

  /**
   * @brief Stop scrolling animation
   */
  void stopScrolling();

private:
  std::string m_fileName;
  std::function<void(const std::string &)> m_fileNameCallback;
  int m_scrollOffset;
  ftxui::ScreenInteractive *m_screen;
  std::thread m_scrollingThread;
  std::atomic<bool> m_scrollActive;

};
} // namespace LahmaPlayer::Gui
