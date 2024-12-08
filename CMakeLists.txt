cmake_minimum_required(VERSION 3.10)

# Project name
project(CANBusExample)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files for CANBus
set(SOURCES_CANBUS
    sources/CANBus.cpp
)

# Create a static library for CANBus
add_library(CANBus STATIC ${SOURCES_CANBUS})

# Specify include directories for the library
target_include_directories(CANBus PUBLIC ./includes) # Include the header directory

# Add the sender executable
add_executable(sender sources/sender.cpp)
target_link_libraries(sender PRIVATE CANBus)  # Link CANBus static library

# Add the receiver executable
add_executable(receiver sources/receiver.cpp)
target_link_libraries(receiver PRIVATE CANBus)  # Link CANBus static library

# Enable testing
enable_testing()
add_test(NAME SendTest COMMAND sender vcan0 123 11 22 33)
add_test(NAME ReceiveTest COMMAND receiver vcan0)

