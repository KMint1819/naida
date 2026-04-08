cmake_minimum_required(VERSION 3.20)

project(naida
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)

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


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(examples)
add_subdirectory(src)
