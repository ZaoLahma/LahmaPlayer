cmake_minimum_required(VERSION 3.10)
project(LahmaPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the external dependencies directory
add_subdirectory(cpp/external)

# Add the core audio library
add_subdirectory(cpp/core)

# Add the GUI library
add_subdirectory(cpp/gui)

# Create the main executable
add_executable(${PROJECT_NAME} 
    cpp/main.cc
)

# Link all required libraries - using the proper target name from FTXUI integration
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    lahmaplayer_core
    Gui
    ftxui_headers
)