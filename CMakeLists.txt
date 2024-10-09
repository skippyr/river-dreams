cmake_minimum_required(VERSION 3.20)
project(river-dreams)
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/libtmk" "${CMAKE_BINARY_DIR}/libtmk")
add_executable(river-dreams "${CMAKE_SOURCE_DIR}/src/river-dreams.c")
target_link_libraries(river-dreams tmk)
if(APPLE)
  target_link_libraries(river-dreams "-framework IOKit"
                                     "-framework CoreFoundation")
endif()
install(TARGETS river-dreams
        DESTINATION "${CMAKE_SOURCE_DIR}/build/bin")
