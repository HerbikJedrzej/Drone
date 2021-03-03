cmake_minimum_required(VERSION 3.10.0)

set(name simulator)

# Add local sources

list(APPEND SIMULATOR_SRC
  Drivers/IMUsensor_simulator.cpp
  Drivers/Memory_simulator.cpp
  Drivers/Radio_simulator.cpp
  DroneVisualisation.cpp
  KeyboardCharProvider.cpp
  KeyboardInterpreter.cpp
  main.cpp
)

# define target

add_executable(${name} ${SIMULATOR_SRC})
target_link_libraries(${name} LINK_PRIVATE CommonCode pthread)
set_property(TARGET ${name} PROPERTY CXX_STANDARD 17)

target_compile_options(${name} PRIVATE
  -pthread
)

# Addlocal headers

target_include_directories(${name} PRIVATE .)
target_include_directories(${name} PRIVATE Drivers)
