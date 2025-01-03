cmake_minimum_required(VERSION 3.10)

project(polyphony VERSION 1.0)

# Include CPM for dependency management
include(FetchContent)
FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
  GIT_TAG v0.40.5
)
FetchContent_MakeAvailable(CPM)

# Include CPM
include(${cpm_SOURCE_DIR}/cmake/CPM.cmake)

# Fetch LLVM
CPMAddPackage(
  NAME LLVM
  GITHUB_REPOSITORY llvm/llvm-project
  GIT_TAG llvmorg-16.0.0
)

message(STATUS "Source Directory: ${LLVM_SOURCE_DIR}")

set(LLVM_CONFIG_PATH "${LLVM_SOURCE_DIR}/build/bin/llvm-config")

# Check if llvm-config exists (indicating the build is complete)
if (EXISTS ${LLVM_CONFIG_PATH})
    message(STATUS "LLVM already built. Skipping build.")
else()
    message(STATUS "Building LLVM from source")

    execute_process(
        COMMAND ${CMAKE_COMMAND} -S ${LLVM_SOURCE_DIR}/llvm -B ${LLVM_SOURCE_DIR}/build
            -DLLVM_TARGETS_TO_BUILD=X86
            -DLLVM_INCLUDE_TESTS=OFF
            -DLLVM_INCLUDE_EXAMPLES=OFF
            -DLLVM_INCLUDE_DOCS=OFF
            -DLLVM_ENABLE_BINDINGS=OFF
            -DCMAKE_BUILD_TYPE=Release
        RESULT_VARIABLE CONFIGURE_RESULT
    )

    if (NOT CONFIGURE_RESULT EQUAL 0)
        message(FATAL_ERROR "LLVM configuration failed!")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${LLVM_SOURCE_DIR}/build -- -j8
        RESULT_VARIABLE BUILD_RESULT
    )

    if (NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "LLVM build failed!")
    endif()
endif()

# Set LLVM directory after build check
message(STATUS "LLVM build completed or already exists.")


# Set LLVM directory
set(LLVM_DIR "${LLVM_SOURCE_DIR}/build/lib/cmake/llvm")

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Include path: ${LLVM_INCLUDE_DIRS}")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Locate source files
file(GLOB SOURCES "src/*.cpp")
add_executable(polyphony ${SOURCES})

# Include directories
target_include_directories(polyphony PUBLIC
    ${LLVM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

# Compile definitions
target_compile_definitions(polyphony PUBLIC ${LLVM_DEFINITIONS})

# Link LLVM components
llvm_map_components_to_libnames(LLVM_LIBS support core irreader codegen)
target_link_libraries(polyphony ${LLVM_LIBS})

# Option to enable profiling
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

# Compiler warnings
if (MSVC)
    target_compile_options(polyphony PRIVATE /W3 /WX)
else()
    target_compile_options(polyphony PRIVATE -Wall -Wextra -Wpedantic -O3)
endif()
