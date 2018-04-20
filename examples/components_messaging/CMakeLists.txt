cmake_minimum_required(VERSION 3.10)
project(ServiceSystemComponentsMessaging)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServiceSystemComponentsMessaging
    src/main.cpp
    src/SenderComponent.cpp
    include/SenderComponent.hpp
    src/ReceiverComponent.cpp
    include/ReceiverComponent.hpp
)

target_include_directories(ServiceSystemComponentsMessaging PUBLIC
    include
)

target_link_libraries(ServiceSystemComponentsMessaging
    ServiceSystem
)