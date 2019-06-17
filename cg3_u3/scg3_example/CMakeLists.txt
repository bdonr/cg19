cmake_minimum_required(VERSION 3.10)
project(scg3_example)

set(CMAKE_CXX_STANDARD 11)

add_executable (scg3_example
    main.cpp
    ../scg3/src/*.cpp
        ../scg3/src/*.h
        )

#include_directories(${CMAKE_SOURCE_DIR}/scg3)

include_directories(.)
include_directories(extern/glm/glm)
include_directories(extern/glm/glm/detail)
include_directories(extern/glm/glm/ext)
include_directories(extern/glm/glm/gtc)
include_directories(extern/glm/glm/gtx)
include_directories(extern/glm/glm/simd)




