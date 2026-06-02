add_library(naida
    weight_loader.cc
    common.cc
    block.cc
    tensor.cc
    tokenizer.cc
)
find_package(Boost REQUIRED COMPONENTS regex)

# uc: common libs for other icu components
# i18n: ICU internalization lib.
# see also https://cmake.org/cmake/help/latest/module/FindICU.html
find_package(ICU REQUIRED COMPONENTS uc i18n)
target_include_directories(naida 
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty
)
target_link_libraries(naida
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC fmt::fmt
    PUBLIC Boost::regex
    PUBLIC ${ICU_LIBRARIES}
)