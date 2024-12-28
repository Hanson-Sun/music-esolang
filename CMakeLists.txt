cmake_minimum_required(VERSION 3.10)

project(polyphony VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")

add_executable(polyphony ${SOURCES})

target_include_directories(polyphony PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# option to enable profiling
option(ENABLE_PROFILING "Enable profiling with gprof" OFF)

if (ENABLE_PROFILING)
    if (MSVC)
        target_compile_options(polyphony PRIVATE /Zi /Gh /GH)
        target_link_options(polyphony PRIVATE /PROFILE)
    else()
        target_compile_options(polyphony PRIVATE -pg)
        target_link_options(polyphony PRIVATE -pg)
    endif()
endif()

# libraries 
# target_link_libraries(polyphony PRIVATE ...)

if (MSVC)
    target_compile_options(polyphony PRIVATE /W3 /WX)
else()
    target_compile_options(polyphony PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()