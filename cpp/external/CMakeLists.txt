add_library(external_headers INTERFACE)

target_include_directories(external_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/minimp3
)