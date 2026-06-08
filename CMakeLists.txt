cmake_minimum_required(VERSION 3.20)

project(naida
    LANGUAGES CXX CUDA
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# STREQUAL is case insensitive
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_compile_definitions(NAIDA_LOG_LEVEL=NAIDA_LOG_LEVEL_TRACE)
else()
  add_compile_definitions(NAIDA_LOG_LEVEL=NAIDA_LOG_LEVEL_INFO)
endif()

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
# See https://cmake.org/cmake/help/latest/policy/CMP0135.html#policy:CMP0135
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz)
FetchContent_MakeAvailable(fmt)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

option(ENABLE_CUBLAS "Whether to use CUBLAS kernels")
if(ENABLE_CUBLAS)
  find_package(CUDAToolkit REQUIRED)
endif()



add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(tests)
