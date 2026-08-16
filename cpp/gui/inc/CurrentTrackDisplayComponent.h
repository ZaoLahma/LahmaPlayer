#pragma once

#include "ftxui/component/component.hpp"
#include <functional>
#include <string>

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

private:
  std::string m_fileName;
  std::function<void(const std::string &)> m_fileNameCallback;
};
} // namespace LahmaPlayer::Gui
