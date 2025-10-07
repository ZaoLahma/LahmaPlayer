# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(LahmaPlayer)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source directories
add_subdirectory(cpp/core)
#add_subdirectory(cpp/frontend)

add_executable(${PROJECT_NAME} cpp/main.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE lahmaplayer_core)