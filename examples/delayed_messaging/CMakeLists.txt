cmake_minimum_required(VERSION 3.10)
project(ServiceSystemDelayedMessaging)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServiceSystemDelayedMessaging
    src/main.cpp
    src/SenderComponent.cpp
    include/SenderComponent.hpp
    src/ReceiverComponent.cpp
    include/ReceiverComponent.hpp
    include/MessageData.hpp
)

target_include_directories(ServiceSystemDelayedMessaging PUBLIC
    include
)

target_link_libraries(ServiceSystemDelayedMessaging
    ServiceSystem
)