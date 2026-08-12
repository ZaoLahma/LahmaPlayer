# cpp/headless/CMakeLists.txt

add_library(headless STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HeadlessAudioManager.cc
)

target_include_directories(headless
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(headless
    PRIVATE
        lahmaplayer_core
)
