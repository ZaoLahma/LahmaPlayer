cmake_minimum_required(VERSION 3.10)
project(Gui)

add_library(Gui
    src/GuiManager.cc
    src/FilePickerComponent.cc
    src/AudioControlsComponent.cc
    src/SystemControlsComponent.cc
    src/CurrentTrackDisplayComponent.cc
)

target_include_directories(Gui
    PUBLIC
    inc
)

# Use the FTXUI that was fetched via FetchContent in cpp/external/ftxui
target_link_libraries(Gui
    PRIVATE
    lahmaplayer_core
    ftxui_headers
)