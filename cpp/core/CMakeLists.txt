# cpp/core/CMakeLists.txt

add_library(lahmaplayer_core STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/file/src/AudioWavFile.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/src/AudioStream.cc
    # Add other core source files here
)

target_include_directories(lahmaplayer_core
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/file/inc
    ${CMAKE_CURRENT_LIST_DIR}/audio/inc
)

target_include_directories(lahmaplayer_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_library(PORTAUDIO_LIB portaudio)
target_link_libraries(lahmaplayer_core ${PORTAUDIO_LIB})