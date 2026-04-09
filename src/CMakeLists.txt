add_library(naida
    weight_loader.cc
    common.cc
    block.cc
    tensor.cc
)
target_include_directories(naida 
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty
)
target_link_libraries(naida
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC fmt::fmt
)