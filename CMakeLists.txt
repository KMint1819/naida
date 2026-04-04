cmake_minimum_required(VERSION 3.20)

project(naida
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(examples)
add_subdirectory(src)
