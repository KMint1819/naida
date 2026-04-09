add_library(naidalib 
    weight_loader.cc
    common.cc
    layer.cc
    tensor.cc
)
target_include_directories(naidalib 
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty
)
target_link_libraries(naidalib 
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC fmt::fmt
)