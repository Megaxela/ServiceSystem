cmake_minimum_required(VERSION 3.10)
project(ServiceSystem)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLES "Build examples" On)

if (${BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

add_library(ServiceSystem STATIC
    src/Component.cpp
    include/Component.hpp
    src/Engine.cpp
    include/Engine.hpp
    src/Message.cpp
    include/Message.hpp
    src/ThreadComponent.cpp
    include/ThreadComponent.hpp
)

target_include_directories(ServiceSystem PUBLIC
    include
)

target_link_libraries(ServiceSystem
    -pthread
)