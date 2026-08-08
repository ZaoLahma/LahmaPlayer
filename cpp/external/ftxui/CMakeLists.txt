include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG        v4.0.0
)

FetchContent_MakeAvailable(ftxui)

# Create a library target for FTXUI headers
add_library(ftxui_headers INTERFACE)
target_link_libraries(ftxui_headers INTERFACE
  ftxui::screen
  ftxui::dom
  ftxui::component
)
target_include_directories(ftxui_headers INTERFACE
  ${ftxui_SOURCE_DIR}/src
  ${ftxui_SOURCE_DIR}/include
)