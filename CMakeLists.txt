cmake_minimum_required(VERSION 3.20)
project(river-dreams)
add_subdirectory("${CMAKE_SOURCE_DIR}/libs/libtmk" "${CMAKE_BINARY_DIR}/libtmk")
add_executable(left-prompt "${CMAKE_SOURCE_DIR}/src/left-prompt.c"
                           "${CMAKE_SOURCE_DIR}/src/left-prompt.h")
add_executable(right-prompt "${CMAKE_SOURCE_DIR}/src/right-prompt.c"
                            "${CMAKE_SOURCE_DIR}/src/right-prompt.h")
target_link_libraries(left-prompt tmk)
if(APPLE)
  target_link_libraries(left-prompt "-framework IOKit"
                                    "-framework CoreFoundation")
endif()
target_link_libraries(right-prompt tmk)
install(TARGETS left-prompt right-prompt
        DESTINATION "${CMAKE_SOURCE_DIR}/build/bin")
