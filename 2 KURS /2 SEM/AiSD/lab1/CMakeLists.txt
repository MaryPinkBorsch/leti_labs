cmake_minimum_required(VERSION 3.0)

project(lab1)

add_executable(lab1 main.cpp MEGA_compressors.cpp compressors/rle.cpp compressors/huffman.cpp compressors/lz77.cpp compressors/lz78.cpp compressors/mtf.cpp compressors/bwt.cpp test_entropy.cpp)

target_include_directories(lab1 PUBLIC ${CMAKE_SOURCE_DIR}/compressors)
target_include_directories(lab1 PUBLIC ${CMAKE_SOURCE_DIR}/serialization)
