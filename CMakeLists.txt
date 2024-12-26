cmake_minimum_required(VERSION 3.10)

project(polyphony VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")

add_executable(polyphony  ${SOURCES})

target_include_directories(polyphony PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# libraries 
# target_link_libraries(polyphony PRIVATE ...)

if (MSVC)
    target_compile_options(polyphony PRIVATE /W3 /WX)
else()
    target_compile_options(polyphony PRIVATE -Wall -Wextra -Wpedantic)
endif()
