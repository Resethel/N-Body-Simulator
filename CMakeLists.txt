cmake_minimum_required(VERSION 3.14)
set(CMAKE_BUILD_TYPE Release)
project(N-Body-Simulator)


# The version number.
set(N-Body-Simulator_VERSION_MAJOR 0)
set(N-Body-Simulator_VERSION_MINOR 3)
set(N-Body-Simulator_VERSION_PATCH 0)


configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Compilation Flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")


# Finds SFML package
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

# Creates the executable
set(EXECUTABLE_NAME "N-Body-Simulator")
add_executable(

        ${PROJECT_NAME}

        src/Main.cpp
        src/Core.hpp
        src/Core.cpp
        src/Utils.hpp

        src/Celestial/Body.hpp
        src/Celestial/Body.cpp
        src/Celestial/Boundary.hpp
        src/Celestial/Boundary.cpp
        src/Celestial/Sim.hpp
        src/Celestial/Sim.cpp

        src/GFX/Effect.hpp
        src/GFX/Effect.cpp
        src/GFX/Explosion.hpp
        src/GFX/Explosion.cpp
        src/GFX/Light.hpp
        src/GFX/Light.cpp
        src/GFX/Trail.hpp
        src/GFX/Trail.cpp
)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${ADDITIONAL_LINK_FLAGS})
