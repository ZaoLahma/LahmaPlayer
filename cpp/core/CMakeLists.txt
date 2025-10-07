# cpp/core/CMakeLists.txt

add_library(lahmaplayer_core STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/file/src/AudioWavFile.cc
    # Add other core source files here
)

target_include_directories(lahmaplayer_core
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/file/inc
)

target_include_directories(lahmaplayer_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)