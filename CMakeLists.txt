cmake_minimum_required (VERSION 2.6)
set (CMAKE_BUILD_TYPE Release)
project (N-Body-Simulator)


# The version number.
set (N-Body-Simulator_VERSION_MAJOR 0)
set (N-Body-Simulator_VERSION_MINOR 1)
set (N-Body-Simulator_VERSION_PATCH 1)


configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# Compilation Flags
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")


# Finds SFML package
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package (SFML REQUIRED graphics window system)


# Creates the executable
set (EXECUTABLE_NAME "N-Body-Simulator")
add_executable(

    ${PROJECT_NAME}

    src/Main.cpp
    src/Core.hpp
    src/Core.cpp
    src/Utils.hpp

    src/Celestial/Body.hpp
    src/Celestial/Body.cpp
    src/Celestial/Sim.hpp
    src/Celestial/Sim.cpp

    src/GFX/Effect.hpp
    src/GFX/Effect.cpp
)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${ADDITIONAL_LINK_FLAGS})
