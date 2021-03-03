cmake_minimum_required(VERSION 3.10.0)
project(DronBoardV2 CXX ASM)

macro(addOptionFlag flag ONorOFF)
  option(Flag_${flag} "Flag ${Flag_${flag}}:" ${ONorOFF})
  message(STATUS "Flag_${flag}: ${Flag_${flag}}")
  if(Flag_${flag})
    list(APPEND COMMON_FLAGS -${flag})
  endif()
endmacro()

addOptionFlag(Wall ON)
addOptionFlag(pedantic ON)
addOptionFlag(Waddress ON)
addOptionFlag(zdefs ON)
addOptionFlag(Wno-psabi ON)
addOptionFlag(Wunused-parameter ON)
addOptionFlag(Wextra ON)
addOptionFlag(Werror OFF)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/CommonDrivers/src")
  message(FATAL_ERROR
    "\nThere is no commonDriversMCU repository."
    "\nTry 'git submodule update --init --recursive'"
  )
endif()

add_subdirectory(src)
add_subdirectory(simulator)
add_subdirectory(moduleTests)
add_subdirectory(unitTests)
add_subdirectory(uartApp)

if(EXISTS "${PROJECT_SOURCE_DIR}/DroneConfiguration/CMakeLists.txt")
  add_subdirectory(DroneConfiguration)
else()
  message(WARNING
    "\nBuild without stm32 target."
    "\nTo build full project pull DroneConfiguration repository or choose branch for correct HW ID."
    "\nTry:"
    "\ngit submodule update --init --recursive"
    "\nor"
    "\ngit checkout HWID_<nr>"
  )
endif()

execute_process(
  COMMAND
    cp -r ${CMAKE_CURRENT_SOURCE_DIR}/dataFiles .
)
