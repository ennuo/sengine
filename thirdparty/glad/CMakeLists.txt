cmake_minimum_required(VERSION 3.22)
project(glad)

add_library(${PROJECT_NAME} STATIC 
    src/glad.c 
    include/glad.h 
    include/KHR/khrplatform.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)