cmake_minimum_required(VERSION 3.12)
project(statphys)

set(CMAKE_CXX_STANDARD 17)

set(SFML_INCLUDE_DIR "/usr/lib/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "/usr/lib/SFML-2.5.1/lib")
set(SFML_DIR "/usr/lib/SFML-2.5.1/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
endif()

add_executable(statphys visualization.cpp text/Menu_button.cpp text/Menu_button.h main.cpp Demonstration.cpp Demonstration.h State.cpp State.h DemonstrationState.cpp DemonstrationState.h MainMenuState.cpp MainMenuState.h)
target_link_libraries (statphys sfml-graphics sfml-window sfml-system)