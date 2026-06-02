cmake_minimum_required(VERSION 3.20)

project(naida
    LANGUAGES CXX CUDA
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)


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
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

option(ENABLE_CUBLAS "Whether to use CUBLAS kernels")
if(ENABLE_CUBLAS)
  find_package(CUDAToolkit REQUIRED)
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(tests)
