add_executable(app
    app.cc
)
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(app PRIVATE naidalib)
target_include_directories(app 
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty
)