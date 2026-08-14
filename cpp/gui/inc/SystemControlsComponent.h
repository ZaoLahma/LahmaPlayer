#pragma once

#include "AudioControlsComponent.h"
#include "AudioSource.h"
#include "AudioSourceFactory.h"
#include "AudioStream.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <atomic>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace LahmaPlayer::Gui
{
class SystemControlsComponent
{
  public:
    SystemControlsComponent();
    ~SystemControlsComponent();

    /**
     * @brief Create the system controls component
     * @return ftxui::Component representing the system controls
     */
    ftxui::Component createComponent();

    /**
     * @brief Set the exit callback
     * @param exitCallback Function to call when exit is requested
     */
    void setExitCallback(std::function<void()> exitCallback);

  private:
    // Callbacks
    std::function<void()> m_exitCallback;
};
} // namespace LahmaPlayer::Gui
