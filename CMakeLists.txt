cmake_minimum_required (VERSION 2.6)
project (N-Body-Simulator)
set (CMAKE_BUILD_TYPE Release)

# The version number.
set (N-Body-Simulator_VERSION_MAJOR 0)
set (N-Body-Simulator_VERSION_MINOR 1)
set (N-Body-Simulator_VERSION_PATCH 1)

#
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)


# Finds SFML package
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package (SFML REQUIRED graphics window system)

# Creates the executable
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
)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${ADDITIONAL_LINK_FLAGS})
