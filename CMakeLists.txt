cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(LahmaPlayer)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug stuff
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

# Add subdirectories (external should come before any targets that use it)
add_subdirectory(cpp/external)
add_subdirectory(cpp/core)
# add_subdirectory(cpp/frontend)

# Define the main executable
add_executable(${PROJECT_NAME} cpp/main.cc)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        lahmaplayer_core
        external_headers
)