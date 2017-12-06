project(ARDUINO_PROJECTS)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../arduino-cmake-minimal/cmake/")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(ArduinoMinimal)

set(SOURCE_FILES main.cpp)

set(LIBRARIES
)

set(EXTRA_CXX_FLAGS_PROJECT "-Wall -Werror")

arduino(stack-painting "${SOURCE_FILES}" "${LIBRARIES}")
