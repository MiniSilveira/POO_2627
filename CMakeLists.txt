cmake_minimum_required(VERSION 3.29)
project(POO_2627)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

add_executable(POO_2627
        Ecra.cpp
        Terreno.cpp
        main.cpp
        Fabrica.cpp
        )
