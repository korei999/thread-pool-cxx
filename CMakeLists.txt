cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(threadPool VERSION 0.2 LANGUAGES CXX)

add_executable(
    run
    src/main.cc
    src/utils.cc
)

target_compile_options(run PRIVATE -Wall -Wextra)

if (CMAKE_BUILD_TYPE MATCHES "Asan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -fsanitize=undefined -fsanitize=address")
    target_link_options(run BEFORE PRIVATE -fsanitize=undefined PRIVATE -fsanitize=address)
endif()

message(STATUS "CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")

set_property(TARGET run PROPERTY CXX_STANDARD 20)
target_compile_definitions(run PRIVATE "-DLOGS") # used for LOG_* macros

# install(TARGETS kmp DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
