add_library(naidalib 
    weight_loader.cc
    common.cc
)
target_include_directories(naidalib PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(naidalib PRIVATE nlohmann_json::nlohmann_json)
message(${PROJECT_SOURCE_DIR}/include)