add_executable(labwork4 main.cpp parser.cpp)

target_link_libraries(labwork4 Hamming)
target_include_directories(labwork4 PUBLIC ${PROJECT_SOURCE_DIR})
