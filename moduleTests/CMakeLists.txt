cmake_minimum_required(VERSION 3.10.0)

set(name moduleTests)

find_package(GTest REQUIRED)
find_package(PythonLibs 3.8)

# Add local sources

list(APPEND TESTS_SRC
  Filtration/AHRS_test.cpp
  Filtration/AltitudeFiltration_test.cpp
  Filtration/ComplementaryFilter_test.cpp
  Filtration/FilterCombination_test.cpp
  Filtration/kalman_test.cpp
  Filtration/StepinMeanFilter_test.cpp
  matplotlib.cpp
  Models.cpp
  modmain.cpp
  PID/PID_axisHmodel.cpp
  PID/simpleInercion_test.cpp
)

# define target

add_executable(${name} ${TESTS_SRC})
target_link_libraries(${name} LINK_PRIVATE ${GTEST_LIBRARIES} ${PYTHON_LIBRARIES} CommonCode pthread)
set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)

target_compile_options(${name} PRIVATE
  -pthread
  -I/usr/include/python3.8
  -lpython3.8
)

# Addlocal headers

target_include_directories(${name} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${name} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${name} PRIVATE .)
target_include_directories(${name} PRIVATE PID)
target_include_directories(${name} PRIVATE Navigation)
target_include_directories(${name} PRIVATE Filtration)

# Create directory

execute_process(
  COMMAND 
    mkdir moduleTestsOutputData moduleTestsOutputData/Filtration moduleTestsOutputData/PID moduleTestsOutputData/Navigation
)
