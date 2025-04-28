cmake_minimum_required(VERSION 3.0)

project(lab2)

add_executable(lab2 main.cpp RGB_YCBCR.cpp PicoPng.cpp downsampling.cpp dct.cpp)

 
target_include_directories(lab2 PUBLIC ${CMAKE_SOURCE_DIR}/serialization) 
