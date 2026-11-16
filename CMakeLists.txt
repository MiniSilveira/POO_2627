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
        Interpretador.h
        Interpretador.cpp
        Maquina.cpp
        Maquinas.cpp
        Comandos.cpp
        Itens.cpp
)

# Programa de testes separado do simulador.
add_executable(TestesSilo
    TestesSilo.cpp
    Ecra.cpp
    Terreno.cpp
    Fabrica.cpp
    Maquina.cpp
    Maquinas.cpp
    Itens.cpp
)
