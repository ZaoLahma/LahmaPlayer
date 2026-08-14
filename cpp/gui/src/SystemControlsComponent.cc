#include "SystemControlsComponent.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

namespace LahmaPlayer::Gui
{
SystemControlsComponent::SystemControlsComponent()
    : m_exitCallback(nullptr)
{}

SystemControlsComponent::~SystemControlsComponent() {}

ftxui::Component SystemControlsComponent::createComponent()
{
    // Create exit button with ASCII symbol
    auto exit_button = ftxui::Button("E Exit",
                                      [this]
                                      {
                                          // Call exit callback if set
                                          if (m_exitCallback)
                                          {
                                              m_exitCallback();
                                          }
                                      });

    // Create the system controls component
    auto system_controls = ftxui::Container::Vertical({exit_button});

    return system_controls;
}

void SystemControlsComponent::setExitCallback(std::function<void()> exitCallback)
{
    m_exitCallback = exitCallback;
}
} // namespace LahmaPlayer::Gui
