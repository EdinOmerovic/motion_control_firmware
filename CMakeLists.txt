cmake_minimum_required(VERSION 3.10)

# Setup compiler
set(CMAKE_C_COMPILER "C:/cygwin64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/cygwin64/bin/g++.exe")
set(CMAKE_C_COMPILER_WORKS 1)
cmake_policy(SET CMP0079 NEW)

# Define the project
project(motion_control_firmware C)

set(TARGET_NAME "my_executable")

# Define the location of your source files
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(DRIVER_DIR ${SRC_DIR}/drivers/)


# Create a list of all the source files needed for the project
set(SOURCES
${SRC_DIR}/main.c
)

# Define the executable and specify which source files it needs
add_executable(${TARGET_NAME} ${SOURCES})

# Add driver subdirectory
add_subdirectory(${DRIVER_DIR})