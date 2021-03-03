cmake_minimum_required(VERSION 3.10.0)

set(name uartApp)

find_package(GTest REQUIRED)
find_package(PythonLibs 2.7)

# Add local sources

list(APPEND SOURCES
  main.cpp
  src/consoleOptions.cpp
  src/matplotlib.cpp
  src/toggleOnUserInput.cpp
  src/uart.cpp
)

# define target

add_executable(${name} ${SOURCES})
target_link_libraries(${name} LINK_PRIVATE ${PYTHON_LIBRARIES} ${GTEST_LIBRARIES} pthread)
set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)

target_compile_options(${name} PRIVATE ${COMMON_FLAGS})
target_compile_options(${name} PRIVATE
  -I/usr/include/python2.7
  -lpython2.7
  -pthread
  -lm
)

# Addlocal headers

target_include_directories(${name} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${name} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${name} PRIVATE inc)

# Create directory and copy content

execute_process(
  COMMAND 
    mkdir ${CMAKE_CURRENT_BINARY_DIR}/outputs ${CMAKE_CURRENT_BINARY_DIR}/outputs/plots ${CMAKE_CURRENT_BINARY_DIR}/outputs/dataFiles
)

execute_process(
  COMMAND
    cp ${CMAKE_CURRENT_SOURCE_DIR}/matlab/Load_data_to_matlab_env.m ${CMAKE_CURRENT_BINARY_DIR}/outputs/.
)
