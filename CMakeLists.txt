cmake_minimum_required(VERSION 3.10)

# Setup compiler
set(CMAKE_C_COMPILER "C:/cygwin64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/cygwin64/bin/g++.exe")
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_NINJA_DEPTYPE_CXX ".")

# Define the project
project(motion_control_firmware C)

# Define the location of your source files
set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(DRIVER_DIR ${SRC_DIR}/drivers/)


# Add driver subdirectory


# Create a list of all the source files needed for the project
set(SOURCES
    ${SRC_DIR}/main.c
)

# Define the executable and specify which source files it needs
add_subdirectory(${DRIVER_DIR})

add_executable(my_executable ${SOURCES})

# Include header files
#target_include_directories(my_executable PRIVATE ${INC_DIR})