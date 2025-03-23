cmake_minimum_required(VERSION 3.0)

project(lab1)

add_executable(lab1 main.cpp compressors/rle.cpp compressors/huffman.cpp compressors/lz77.cpp)

target_include_directories(lab1 PUBLIC ${CMAKE_SOURCE_DIR}/compressors)
