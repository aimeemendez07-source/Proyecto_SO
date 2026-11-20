cmake_minimum_required(VERSION 3.21)

# Nombre del proyecto y lenguaje (C en este caso)
project(SimuladorSO LANGUAGES C)

# Configurar el estándar de C (C11 es moderno y seguro)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuración por defecto en Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Tipo de compilación" FORCE)
endif()

# Aquí agregarás todos los archivos .c que vayas creando
# Por ahora solo tienes el main.c en la carpeta src
add_executable(simulador 
    src/main.c
)

# Para que puedas usar #include "tus_archivos.h" desde la carpeta include
target_include_directories(simulador PRIVATE include)