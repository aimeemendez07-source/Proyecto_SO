cmake_minimum_required(VERSION 3.21)
project(SimuladorSO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configura el modo Debug por defecto si no se especifica
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Tipo de compilación" FORCE)
endif()

add_executable(simulador 
    src/main.cpp
    src/Procesos.cpp
)

target_include_directories(simulador PRIVATE include)