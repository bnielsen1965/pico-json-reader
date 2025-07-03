
add_subdirectory(jsmn build)

add_library(pico-json-reader INTERFACE)

target_sources(pico-json-reader INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/pico-json-reader.c
)

target_include_directories(pico-json-reader INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/include
)

target_link_libraries(pico-json-reader INTERFACE
  pico_stdlib
  jsmn
)
